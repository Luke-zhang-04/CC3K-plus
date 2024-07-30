#include "game.h"

#include "barrier_suit.h"
#include "compass.h"
#include "constants.h"
#include "dragon.h"
#include "enemy.h"
#include "interactible.h"
#include "merchant.h"
#include "player.h"
#include "potion.h"
#include "random.h"
#include "retrievable.h"
#include "tile.h"
#include "treasure.h"
#include "util.h"

#include <array>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

Game::Game(Player* player, std::ostream& output, std::istream& layoutInput, bool shouldGenerate):
    output{output}, layoutInput{layoutInput}, player{player}, shouldGenerate{shouldGenerate} {
    nextLevel();
}

Game::~Game() {
    delete currentBoard;
    delete player;
}

void Game::update() {
    currentBoard->updateEnemies();
    render();
}

Tile* getTileFromChar(char character, Player* player, Board& board) {
    switch (character) {
        case Symbol::WallVert:
        case Symbol::WallHorz:
        case Symbol::Door:
        case Symbol::Passage:
        case Symbol::FloorTile:
        case Symbol::Blank: return new Tile{character};

        case Symbol::Stairs: return new Tile{Symbol::FloorTile};

        case Symbol::Player: return new Tile{Symbol::FloorTile, nullptr, nullptr, nullptr, player};

        case Symbol::BarrierSuit: return new Tile{Symbol::FloorTile, new BarrierSuit{nullptr}};
        case Symbol::Compass: return new Tile{Symbol::FloorTile, new Compass{board}};

        case Symbol::Vampire:
            return new Tile{
                Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Vampire, board}
            };
        case Symbol::Werewolf:
            return new Tile{
                Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Werewolf, board}
            };
        case Symbol::Troll:
            return new Tile{
                Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Troll, board}
            };
        case Symbol::Goblin:
            return new Tile{
                Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Goblin, board}
            };
        case Symbol::Phoenix:
            return new Tile{
                Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Phoenix, board}
            };

        case Symbol::Merchant:
            return new Tile{Symbol::FloorTile, nullptr, nullptr, new Merchant{board}};
        case Symbol::Dragon:
            return new Tile{Symbol::FloorTile, nullptr, nullptr, new Dragon{board}};

        case (char) InputMapNumbers::PotionRestoreHealth:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{"RH", 10}};
        case (char) InputMapNumbers::PotionBoostAttack:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{"BA", 0, 5}};
        case (char) InputMapNumbers::PotionBoostDefense:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{"BD", 0, 0, 5}};
        case (char) InputMapNumbers::PotionPoisonHealth:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{"PH", -10}};
        case (char) InputMapNumbers::PotionWoundAttack:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{"WA", 0, -5}};
        case (char) InputMapNumbers::PotionWoundDefense:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{"WD", 0, 0, -5}};
        case (char) InputMapNumbers::TreasureSmallGoldPile:
            return new Tile{Symbol::FloorTile, new Treasure{1}};
        case (char) InputMapNumbers::TreasureSmallHoard:
            return new Tile{Symbol::FloorTile, new Treasure{2}};
        case (char) InputMapNumbers::TreasureMerchantHoard:
            return new Tile{Symbol::FloorTile, new Treasure{4}};
        case (char) InputMapNumbers::TreasureDragonHoard:
            return new Tile{Symbol::FloorTile, new Treasure{6}};

        default:
            throw std::invalid_argument("unknown map tile character " + std::string(1, character));
    }
}

std::vector<coordPair>& Game::traverseChamber(
    coordPair&& tile,
    const Board* const newBoard,
    size_t currentChamberId,
    std::vector<coordPair>& recursiveChambers
) {
    auto area = newBoard->getArea(tile);

    newBoard->at(tile)->chamberId = currentChamberId;
    recursiveChambers.emplace_back(tile);

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if ((row == 1 && col == 1) || area[row][col] == nullptr ||
                area[row][col]->mapTile != Symbol::FloorTile || area[row][col]->chamberId != 0) {
                continue;
            }

            // We have an unlabeled floor tile
            traverseChamber(
                {tile.first + col - 1, tile.second + row - 1},
                newBoard,
                currentChamberId,
                recursiveChambers
            );
        }
    }

    return recursiveChambers;
}

std::vector<std::vector<coordPair>>
    Game::labelChambers(std::vector<coordPair>& floorTiles, const Board* const newBoard) {
    size_t chamberIdCounter = 1;

    std::vector<std::vector<coordPair>> chambers;

    for (auto& floorTile : floorTiles) {
        if (newBoard->at(floorTile)->chamberId == 0) {
            // We have an unlabeled boy, label him and his comrades
            chambers.emplace_back(std::vector<coordPair>{});
            traverseChamber(
                std::move(floorTile), newBoard, chamberIdCounter++, chambers[chambers.size() - 1]
            );
        }
    }

    return chambers;
}

void Game::randomPopulateMap(Board* newBoard, Player* player) {
    std::vector<std::vector<coordPair>>& chambers = newBoard->chambers;
    size_t totalTileCount = 0;
    // Pair (chamber id, remaining spawnable tiles)
    std::vector<std::pair<size_t, size_t>> chamberTileCounts(chambers.size());
    // Number of things have been spawned in each chamber
    std::unordered_map<size_t, std::vector<coordPair>::iterator> chamberIters{};
    std::queue<size_t> selectedChambers{}; // Chambers selected for spawning

    // Spawn distributions
    std::array<Potion, 6> potionDist{
        Potion{"RH", 10}, Potion{"BH", 0, 5}, Potion{"BD", 0, 0, 5}, Potion{"PH", -10}, Potion{"WA", 0, -5}, Potion{"WD", 0, 0, -5}
    };
    std::vector<int> goldDist = createVector(std::array<std::pair<size_t, int>, 3>{{
        {SpawnRates::GoldNormalRate, 2},
        {SpawnRates::GoldSmallHoardRate, 1},
        {SpawnRates::GoldDragonHoardRate, 6},
    }});
    std::vector<EnemyType> enemyDist = createVector(std::array<std::pair<size_t, EnemyType>, 6>{
        {{SpawnRates::EnemyVampireRate, EnemyType::Vampire},
         {SpawnRates::EnemyWerewolfRate, EnemyType::Werewolf},
         {SpawnRates::EnemyTrollRate, EnemyType::Troll},
         {SpawnRates::EnemyGoblinRate, EnemyType::Goblin},
         {SpawnRates::EnemyMerchantRate, EnemyType::Merchant},
         {SpawnRates::EnemyPhoenixRate, EnemyType::Phoenix}}
    });

    // Summary: selectedChambers holds chamberIDs in some random order. The number of occurrences
    // of a chamberID is bounded by chamberTileCounts. Each spawned item advances
    // chamberIters. We shuffle chamberTiles, so this is random. to index the tiles.

    chamberIters.reserve(chambers.size());

    for (size_t index = 0; index < chambers.size(); index++) {
        shuffle(chambers[index]);

        totalTileCount += chambers[index].size();
        chamberTileCounts[index] = {newBoard->at(chambers[index][0])->chamberId, chambers[index].size()};
        chamberIters[index] = chambers[index].begin();
    }
    if (totalTileCount <= SpawnRates::Total + 2) {
        throw std::length_error("board is not large enough to spawn everything");
    }

    std::pair<int, int> initSpawns = randIntPair(0, chambers.size() - 1);

    newBoard->at(*chamberIters[initSpawns.first])->player = player;
    newBoard->playerLocation = *chamberIters[initSpawns.first];
    newBoard->stairLocation = *chamberIters[initSpawns.second];

    chamberIters[initSpawns.first]++;
    chamberIters[initSpawns.second]++;

    for (uint8_t i = 0; i < SpawnRates::Total; i++) {
        size_t randIndex = randInt(0, chamberTileCounts.size() - 1);

        chamberTileCounts[randIndex].second--;

        selectedChambers.push(chamberTileCounts[randIndex].first);

        if (chamberTileCounts[randIndex].second == 0) {
            chamberTileCounts.erase(chamberTileCounts.begin() + randIndex);
        }
    }

    for (uint8_t i = 0; i < SpawnRates::PotionTotal; i++) {
        size_t chamberId = selectedChambers.front();
        Potion* potion = new Potion{potionDist[randInt(0, potionDist.size() - 1)]};

        selectedChambers.pop();

        newBoard->at(*chamberIters[chamberId])->item = potion;
        chamberIters[chamberId]++;
    }
    for (uint8_t i = 0; i < SpawnRates::GoldTotal; i++) {
        size_t chamberId = selectedChambers.front();
        int amt = goldDist[randInt(0, goldDist.size() - 1)];

        selectedChambers.pop();

        if (amt == 6) {
            // TODO DRAGON HOW???
        } else {
            newBoard->at(*chamberIters[chamberId])->treasure = new Treasure{amt};
            chamberIters[chamberId]++;
        }
    }
    for (uint8_t i = 0; i < SpawnRates::EnemyTotal; i++) {
        size_t chamberId = selectedChambers.front();
        EnemyType type = enemyDist[randInt(0, enemyDist.size() - 1)];

        selectedChambers.pop();

        newBoard->at(*chamberIters[chamberId])->enemy = new Enemy{type, *newBoard};
        chamberIters[chamberId]++;
    }
}

/**
 * @brief delete the old board and create a new one using data from the layoutInput stream
 *
 * Assumptions:
 * - The map contains the `@` symbol at most once
 * - There will be exactly one dragon in a cardinal or ordinal direction from the hoard
 *     - No need to use bipartite matching algorithm
 * - https://piazza.com/class/lvkzhr2nagi58v/post/294
 */
void Game::nextLevel() {
    coordPair compassLocation{0, 0};
    coordPair stairLocation{0, 0};
    coordPair playerLocation{0, 0};
    // TODO: there can actually be multiple dragons
    Dragon* dragon = nullptr;                       // NON OWNERSHIP
    DragonProtected* dragonProtectedItem = nullptr; // NON OWNERSHIP
    std::vector<coordPair> floorTiles;              // Keep track of floor tiles for spawning
    std::vector<Enemy*> compassHoldingEnemies;      // Enemies that CAN hold a compass

    Board* newBoard = new Board{std::vector<std::vector<Tile*>>(1, std::vector<Tile*>{}), *this};

    char input;
    bool isEnd = false; // Tracks if a line has only | and -, but not the first line

    std::vector<std::vector<Tile*>>& tiles = newBoard->map;

    layoutInput >> std::noskipws;

    while (layoutInput >> input) {
        std::vector<Tile*>& row = tiles[tiles.size() - 1];

        if (input == '\n') {
            if (isEnd) { // End of floor
                break;
            }

            tiles.emplace_back(std::vector<Tile*>{});
            isEnd = true;
            std::cout << std::endl;
            continue;
        }
        if (input != Symbol::WallHorz && input != Symbol::WallVert) {
            isEnd = false;
        }

        row.emplace_back(getTileFromChar(input, player, *newBoard));
        coordPair currentLocation{row.size() - 1, tiles.size() - 1};

        if (input == Symbol::FloorTile) {
            floorTiles.push_back(currentLocation);
        }
        if (input == Symbol::Stairs) {
            stairLocation = currentLocation;
        }
        if (input == Symbol::Player) {
            playerLocation = currentLocation;
        }
        // "You can assume the file won't contain a compass."
        // --(https://piazza.com/class/lvkzhr2nagi58v/post/294)
        if (input == Symbol::Compass) {
            compassLocation = currentLocation;
        }
        if (input == Symbol::BarrierSuit || input == (char) InputMapNumbers::TreasureDragonHoard) {
            dragonProtectedItem = dynamic_cast<DragonProtected*>(row[row.size() - 1]->treasure);
        }
        if (input == Symbol::Dragon) {
            dragon = dynamic_cast<Dragon*>(row[row.size() - 1]->enemy);
        }
        if (isEnemy(input) && input != Symbol::Merchant && input != Symbol::Dragon) {
            compassHoldingEnemies.push_back(row[row.size() - 1]->enemy);
        }
    }

    layoutInput >> std::skipws;

    // Postprocessing: connect dragon to protected
    if (dragonProtectedItem != nullptr && dragon != nullptr) {
        if (dragon->protects != nullptr || dragonProtectedItem->dragon != nullptr) {
            throw std::logic_error(
                "dragon/dragon protected item already linked (that's not supposed to happen wtf?)"
            );
        }

        dragon->protects = dragonProtectedItem;
        dragonProtectedItem->dragon = dragon; // DRAGON DEEZ NUTS ACROSS...
    }

    // Postprocessing: label chambers
    // This operates in-place with side effects on `newBoard`
    newBoard->chambers = labelChambers(floorTiles, newBoard);

    if (shouldGenerate) { // Random gen
        randomPopulateMap(newBoard, player);
    } else { // Passed in map
        if (stairLocation.first == 0 || stairLocation.second == 0) {
            throw std::invalid_argument("invalid stair location in pre-determined map");
        }
        if (playerLocation.first == 0 || playerLocation.second == 0) {
            throw std::invalid_argument("invalid player location in pre-determined map");
        }
        if (compassHoldingEnemies.size() == 0 &&
            (compassLocation.first == 0 || compassLocation.second == 0)) {
            throw std::invalid_argument("cannot assign compass to enemy and location not given");
        }

        newBoard->stairLocation = stairLocation;
        newBoard->playerLocation = playerLocation;

        if (compassLocation.first == 0 || compassLocation.second == 0) {
            size_t index = randInt(0, compassHoldingEnemies.size() - 1);

            compassHoldingEnemies[index]->giveTreasure(new Compass{*newBoard});
        } else {
            newBoard->at(compassLocation)->treasure = new Compass{*newBoard};
        }
    }

    delete currentBoard;
    currentBoard = newBoard;

    ++level;

    // DEBUG:
    currentBoard->showStairs();
}

bool Game::playerMove(CardinalDirection dir) {
    // current player location
    auto loc = currentBoard->getPlayerLoc();

    // TODO: can I put an assertion here that player is not nullptr? It should always be defined
    // and things have gone very wrong if not
    Tile* curTile = currentBoard->at(loc);
    Player* player = curTile->player;
    Tile* newTile = currentBoard->inDirection(loc, dir);

    // now check if we landed on a pickup
    if (newTile->treasure) {
        // attempt to have the player retrieve it
        if (newTile->treasure->onRetrieve(player)) {
            // if they did, delete the Retrievable
            delete newTile->treasure;
            newTile->treasure = nullptr;
        }
    }

    // if we can move into the desired location
    if (player->canMove(newTile)) {
        // move the player to the open location
        std::swap(curTile->player, newTile->player);
        currentBoard->movePlayer(dir);

        // logging logic
        player->log() << "PC moves " << directionToString(dir);
        auto newArea = currentBoard->getArea(loc);
        for (int dx = 0; dx <= 2; ++dx) {
            for (int dy = 0; dy <= 2; ++dy) {
                if (dx == 1 && dy == 1)
                    continue; // if both are 0, move on, don't let them not move

                if (newArea[dy][dx] != nullptr) {
                    if (newArea[dy][dx]->enemy)
                        player->log() << " and sees a " << newArea[dy][dx]->getCharacter();
                    else if (newArea[dy][dx]->item)
                        player->log() << " and sees an unknown potion ";
                    else if (newArea[dy][dx]->treasure)
                        player->log() << " and sees a treasure";
                }
            }
        }
        player->log() << ".";
    }


    update();

    // logging

    return player->getHealth() > 0;
}

bool Game::playerAttack(CardinalDirection dir) {
    // find the target location
    auto loc = currentBoard->getPlayerLoc();
    Tile* targetTile = currentBoard->inDirection(loc, dir);

    // if there is an enemy there, they are attacked
    Enemy* enemy = targetTile->enemy;
    if (enemy) {
        std::pair<int, int> attackStats = enemy->beAttacked(player->getPower());
        player->log() << "PC deals " << attackStats.second << " damage to "
                      << targetTile->getCharacter() << " (" << std::max(0, attackStats.first)
                      << " HP). ";
        if (attackStats.first < 0) {
            // if they die, take their gold
            player->pickupGold(enemy->goldValue());

            // if they have an item, drop it
            targetTile->treasure = enemy->dropTreasure();

            // they no longer have the item, so delete them and the item is safe
            delete enemy;
            targetTile->enemy = nullptr;
        }
    }

    // update everything else, possibly triggering the enemy to attack back
    update();

    // player->log("PC deals " + )

    return player->getHealth() > 0;
}

bool Game::playerPickup(CardinalDirection dir) {
    // find the location they want to interact with
    auto loc = currentBoard->getPlayerLoc();
    Tile* targetTile = currentBoard->inDirection(loc, dir);

    // if there is an item there, interact with it
    Interactible* item = targetTile->item;
    if (item) {
        // use the item
        item->onInteract(player);

        // then delete it
        delete item;
        targetTile->item = nullptr;
    }

    // update everything else
    update();

    return player->getHealth() > 0;
}

void Game::render() const {
    currentBoard->render(output);
    player->displayInfo(output);
    // TODO: info pannel
}
