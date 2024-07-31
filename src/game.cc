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
#include "types.h"
#include "util.h"

#include <array>
#include <cstddef>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::size_t;

Game::Game(Player* player, std::ostream& output, std::istream& layoutInput, bool shouldGenerate):
    output{output}, layoutInput{layoutInput}, player{player}, shouldGenerate{shouldGenerate},
    suitLevel{(int) randInt(0, 4)} {
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

Tile* getTileFromChar(coordPair loc, char character, Player* player, Board& board) {
    switch (character) {
        case Symbol::WallVert:
        case Symbol::WallHorz:
        case Symbol::Door:
        case Symbol::Passage:
        case Symbol::FloorTile:
        case Symbol::Blank: return new Tile{loc, character};

        case Symbol::Stairs: return new Tile{loc, Symbol::FloorTile};

        case Symbol::Player:
            return new Tile{loc, Symbol::FloorTile, nullptr, nullptr, nullptr, player};

        case Symbol::BarrierSuit:
            return new Tile{loc, Symbol::FloorTile, new BarrierSuit{nullptr}};
        case Symbol::Compass: return new Tile{loc, Symbol::FloorTile, new Compass{board}};

        case Symbol::Vampire:
            return new Tile{
                loc, Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Vampire, board}
            };
        case Symbol::Werewolf:
            return new Tile{
                loc, Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Werewolf, board}
            };
        case Symbol::Troll:
            return new Tile{
                loc, Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Troll, board}
            };
        case Symbol::Goblin:
            return new Tile{
                loc, Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Goblin, board}
            };
        case Symbol::Phoenix:
            return new Tile{
                loc, Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Phoenix, board}
            };

        case Symbol::Merchant:
            return new Tile{loc, Symbol::FloorTile, nullptr, nullptr, new Merchant{board}};
        case Symbol::Dragon:
            return new Tile{loc, Symbol::FloorTile, nullptr, nullptr, new Dragon{board}};

        case (char) InputMapNumbers::PotionRestoreHealth:
            return new Tile{loc, Symbol::FloorTile, nullptr, new Potion{"RH", 10}};
        case (char) InputMapNumbers::PotionBoostAttack:
            return new Tile{loc, Symbol::FloorTile, nullptr, new Potion{"BA", 0, 5}};
        case (char) InputMapNumbers::PotionBoostDefense:
            return new Tile{loc, Symbol::FloorTile, nullptr, new Potion{"BD", 0, 0, 5}};
        case (char) InputMapNumbers::PotionPoisonHealth:
            return new Tile{loc, Symbol::FloorTile, nullptr, new Potion{"PH", -10}};
        case (char) InputMapNumbers::PotionWoundAttack:
            return new Tile{loc, Symbol::FloorTile, nullptr, new Potion{"WA", 0, -5}};
        case (char) InputMapNumbers::PotionWoundDefense:
            return new Tile{loc, Symbol::FloorTile, nullptr, new Potion{"WD", 0, 0, -5}};
        case (char) InputMapNumbers::TreasureSmallGoldPile:
            return new Tile{loc, Symbol::FloorTile, new Treasure{TreasureType::SmallGoldPile}};
        case (char) InputMapNumbers::TreasureSmallHoard:
            return new Tile{loc, Symbol::FloorTile, new Treasure{TreasureType::SmallHoard}};
        case (char) InputMapNumbers::TreasureMerchantHoard:
            return new Tile{loc, Symbol::FloorTile, new Treasure{TreasureType::MerchantHoard}};
        case (char) InputMapNumbers::TreasureDragonHoard:
            return new Tile{loc, Symbol::FloorTile, new Treasure{TreasureType::DragonHoard}};

        default:
            throw std::invalid_argument{"unknown map tile character " + std::string(1, character)};
    }
}

std::vector<Tile*>& Game::traverseChamber(
    Tile* tile,
    const Board* const newBoard,
    size_t currentChamberId,
    std::vector<Tile*>& recursiveChambers
) {
    auto area = newBoard->getArea(tile);

    tile->chamberId = currentChamberId;
    recursiveChambers.emplace_back(tile);

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if ((row == 1 && col == 1) || area[row][col] == nullptr ||
                area[row][col]->mapTile != Symbol::FloorTile || area[row][col]->chamberId != 0) {
                continue;
            }

            // We have an unlabeled floor tile. Label it and add it.
            traverseChamber(
                // Workaround because `getArea` returns constants but `at` is private and does not
                newBoard->at(area[row][col]->location),
                newBoard,
                currentChamberId,
                recursiveChambers
            );
        }
    }

    return recursiveChambers;
}

std::vector<std::vector<Tile*>>
    Game::labelChambers(std::vector<Tile*>& floorTiles, const Board* const newBoard) {
    size_t chamberIdCounter = 1;

    std::vector<std::vector<Tile*>> chambers;

    for (auto& floorTile : floorTiles) {
        if (floorTile->chamberId == 0) {
            // We have an unlabeled boy, label him and his comrades
            chambers.emplace_back(std::vector<Tile*>{});
            traverseChamber(
                std::move(floorTile), newBoard, chamberIdCounter++, chambers[chambers.size() - 1]
            );
        }
    }

    return chambers;
}

// TODO: generate barrier suit
void Game::randomPopulateMap(Board* newBoard, Player* player, bool shouldGenerateSuit) {
    std::vector<std::vector<Tile*>>& chambers = newBoard->chambers;
    size_t totalTileCount = 0;
    // Pair (chamber id, remaining spawnable tiles)
    // When remaining spawnable tiles becomes 0, delete that pair from the vector. This way, we can
    // always pick a random chamber that has a free tile.
    std::vector<coordPair> chamberTileCounts(chambers.size());
    // Number of things have been spawned in each chamber
    std::vector<std::vector<Tile*>::iterator> chamberIters{};
    std::queue<size_t> selectedChambers{}; // Chambers selected for spawning
    std::vector<Enemy*> compassHoldingEnemies{};

    compassHoldingEnemies.reserve(SpawnRates::EnemyTotal);
    chamberIters.reserve(chambers.size());

    // Spawn distributions
    std::array<Potion, 6> potionDist{
        Potion{"RH", 10},
        Potion{"BH", 0, 5},
        Potion{"BD", 0, 0, 5},
        Potion{"PH", -10},
        Potion{"WA", 0, -5},
        Potion{"WD", 0, 0, -5}
    };
    std::vector<uint8_t> goldDist = createVector(std::array<std::pair<size_t, uint8_t>, 3>{{
        {SpawnRates::GoldNormalRate, TreasureType::SmallGoldPile},
        {SpawnRates::GoldSmallHoardRate, TreasureType::SmallHoard},
        {SpawnRates::GoldDragonHoardRate, TreasureType::DragonHoard},
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

    for (size_t index = 0; index < chambers.size(); index++) {
        shuffle(chambers[index]);

        totalTileCount += chambers[index].size() - 1;
        size_t offsetChamberId = chambers[index][0]->chamberId - 1;

        chamberTileCounts[offsetChamberId] = {
            offsetChamberId, chambers[offsetChamberId].size() - 1
        };
        chamberIters[offsetChamberId] = chambers[index].begin();
    }

    // +2 for player/stairs, +1 for potential compass, +2 for potential barrier suit, +goldTotal
    // for dragon hoards
    if (totalTileCount <= SpawnRates::Total + SpawnRates::GoldTotal + 3) {
        throw std::length_error{
            "board is not large enough to spawn everything (want " +
            std::to_string(SpawnRates::Total + SpawnRates::GoldTotal + 3) + ", got " +
            std::to_string(totalTileCount) + ")"
        };
    }

    // (random player location, random stair location)
    std::pair<size_t, size_t> initSpawns = randIntPair(0, chambers.size() - 1);

    (*chamberIters[initSpawns.first])->player = player;
    newBoard->playerLocation = (*chamberIters[initSpawns.first])->location;
    chamberIters[initSpawns.first]++;
    newBoard->stairLocation = (*chamberIters[initSpawns.second])->location;
    chamberIters[initSpawns.second]++;

    // Generate barrier suit
    if (shouldGenerateSuit) {
        size_t chamberId = randInt(0, chambers.size() - 1);

        Tile* tile = *chamberIters[chamberId];
        chamberIters[chamberId]++;
        auto area = newBoard->getArea(tile);

        std::vector<std::pair<uint8_t, uint8_t>> available{};

        available.reserve(8);

        for (uint8_t dx = 0; dx <= 2; dx++) {
            for (uint8_t dy = 0; dy <= 2; dy++) {
                if (dx == 1 && dy == 1)
                    continue;

                const Tile* tile = area[dy][dx];

                if (tile != nullptr && tile->empty() && tile->mapTile == Symbol::FloorTile) {
                    available.push_back({dx, dy});
                }
            }
        }

        if (available.size() == 0) {
            tile->treasure = new BarrierSuit{nullptr};
        } else {
            std::pair<uint8_t, uint8_t> dragonOffset = available[randInt(0, available.size() - 1)];
            Tile* dragonTile =
                newBoard->at(area[dragonOffset.second][dragonOffset.first]->location);
            Dragon* dragon = new Dragon{*newBoard};
            BarrierSuit* treasure = new BarrierSuit{dragon};

            dragon->protects = treasure;

            tile->treasure = treasure;
            dragonTile->enemy = dragon;
        }
    }

    // Select chambers randomly
    for (uint8_t i = 0; i < SpawnRates::Total + SpawnRates::GoldTotal + 1; i++) {
        size_t randIndex = randInt(0, chamberTileCounts.size() - 1);

        chamberTileCounts[randIndex].second--;

        selectedChambers.push(chamberTileCounts[randIndex].first);

        if (chamberTileCounts[randIndex].second == 0) {
            chamberTileCounts.erase(chamberTileCounts.begin() + randIndex);
        }
    }

    // Use all the data we just collected to spawn stuff (steps should be relatively simple):
    // 1. Randomly pick the type of item spawned based on the distribution arrays
    // 2. Pop the next pre-selected random chamber ID from the queue
    // 3. Get the next tile in the iterator of that chamber ID (recall the tiles were shuffled)
    // 4. Repeat steps 2 and 3 until there is a free tile (the only case where there isn't a free
    //    tile is when the dragon associated with a barrier suit/dragon hoard was placed)
    // 5. Add item to tile

    for (uint8_t i = 0; i < SpawnRates::PotionTotal; i++) {
        Potion* potion = new Potion{potionDist[randInt(0, potionDist.size() - 1)]};

        size_t chamberId = selectedChambers.front();
        selectedChambers.pop();

        Tile* tile = *chamberIters[chamberId];
        chamberIters[chamberId]++;

        while (!tile->empty()) {
            chamberId = selectedChambers.front();
            selectedChambers.pop();
            tile = *chamberIters[chamberId];
            chamberIters[chamberId]++;
        }

        tile->item = potion;
    }
    for (uint8_t i = 0; i < SpawnRates::GoldTotal; i++) {
        int amt = goldDist[randInt(0, goldDist.size() - 1)];

        size_t chamberId = selectedChambers.front();
        selectedChambers.pop();

        Tile* tile = *chamberIters[chamberId];
        chamberIters[chamberId]++;

        while (!tile->empty()) {
            chamberId = selectedChambers.front();
            selectedChambers.pop();
            tile = *chamberIters[chamberId];
            chamberIters[chamberId]++;
        }

        if (amt == TreasureType::DragonHoard) {
            auto area = newBoard->getArea(tile);

            std::vector<std::pair<uint8_t, uint8_t>> available{};

            available.reserve(8);

            for (uint8_t dx = 0; dx <= 2; dx++) {
                for (uint8_t dy = 0; dy <= 2; dy++) {
                    if (dx == 1 && dy == 1)
                        continue;

                    const Tile* tile = area[dy][dx];

                    if (tile != nullptr && tile->empty() && tile->mapTile == Symbol::FloorTile) {
                        available.push_back({dx, dy});
                    }
                }
            }

            if (available.size() == 0) {
                tile->treasure = new Treasure{amt};
            } else {
                std::pair<uint8_t, uint8_t> dragonOffset =
                    available[randInt(0, available.size() - 1)];
                Tile* dragonTile =
                    newBoard->at(area[dragonOffset.second][dragonOffset.first]->location);
                Dragon* dragon = new Dragon{*newBoard};
                Treasure* treasure = new Treasure{TreasureType::DragonHoard};

                treasure->setProtector(dragon);
                dragon->protects = treasure;

                tile->treasure = treasure;
                dragonTile->enemy = dragon;
            }
        } else {
            tile->treasure = new Treasure{amt};
        }
    }
    for (uint8_t i = 0; i < SpawnRates::EnemyTotal; i++) {
        EnemyType type = enemyDist[randInt(0, enemyDist.size() - 1)];

        size_t chamberId = selectedChambers.front();
        selectedChambers.pop();

        Tile* tile = *chamberIters[chamberId];
        chamberIters[chamberId]++;

        while (!tile->empty()) {
            chamberId = selectedChambers.front();
            selectedChambers.pop();
            tile = *chamberIters[chamberId];
            chamberIters[chamberId]++;
        }

        Enemy* newEnemy = new Enemy{type, *newBoard};
        tile->enemy = newEnemy;

        // Should never be dragon but check anyways, merchant is definitely possible, and we should
        // not give merchants the compass
        if (type != EnemyType::Dragon && type != EnemyType::Merchant) {
            compassHoldingEnemies.push_back(newEnemy);
        }
    }

    if (compassHoldingEnemies.empty()) {
        // If there is no enemy to hold the compass, put it on the ground somewhere
        size_t chamberId = selectedChambers.front();

        selectedChambers.pop();
        (*chamberIters[chamberId])->treasure = new Compass{*newBoard};
        chamberIters[chamberId]++;
    } else {
        Enemy* compassHolder = compassHoldingEnemies[randInt(0, compassHoldingEnemies.size() - 1)];

        compassHolder->giveTreasure(new Compass{*newBoard});
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
    // reset the player, just in case this isn't the first floor

    // if we are calling nextLevel from the last level, do nothing (you win)
    if (level >= levelCount)
        return;

    player->reset();

    coordPair compassLocation{0, 0};
    coordPair stairLocation{0, 0};
    coordPair playerLocation{0, 0};
    // TODO: there can actually be multiple dragons
    Dragon* dragon = nullptr;                   // NON OWNERSHIP
    Retrievable* dragonProtectedItem = nullptr; // NON OWNERSHIP
    std::vector<Tile*> floorTiles;              // Keep track of floor tiles for spawning
    std::vector<Enemy*> compassHoldingEnemies;  // Enemies that CAN hold a compass

    Board* newBoard = new Board{std::vector<std::vector<Tile*>>(1, std::vector<Tile*>{}), *this};

    char input;
    bool isEnd = false; // Tracks if a line has only | and -, but not the first line

    std::vector<std::vector<Tile*>>& tiles = newBoard->map;

    while (layoutInput >> input) {
        std::vector<Tile*>& row = tiles[tiles.size() - 1];

        if (input == '\n') {
            if (isEnd) { // End of floor
                break;
            }

            tiles.emplace_back(std::vector<Tile*>{});
            isEnd = true;
            continue;
        }
        if (input != Symbol::WallHorz && input != Symbol::WallVert) {
            isEnd = false;
        }

        coordPair currentLocation{row.size(), tiles.size() - 1};
        Tile* newTile = getTileFromChar(currentLocation, input, player, *newBoard);
        row.emplace_back(newTile);

        if (input == Symbol::FloorTile) {
            floorTiles.push_back(newTile);
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
            dragonProtectedItem = row[row.size() - 1]->treasure;
        }
        if (input == Symbol::Dragon) {
            dragon = dynamic_cast<Dragon*>(row[row.size() - 1]->enemy);
        }
        if (isEnemy(input) && input != Symbol::Merchant && input != Symbol::Dragon) {
            compassHoldingEnemies.push_back(row[row.size() - 1]->enemy);
        }
    }

    // Postprocessing: connect dragon to protected
    if (dragonProtectedItem != nullptr && dragon != nullptr) {
        if (dragon->protects != nullptr || dragonProtectedItem->isProtected()) {
            throw std::logic_error{
                "dragon/dragon protected item already linked (that's not supposed to happen wtf?)"
            };
        }

        dragon->protects = dragonProtectedItem;
        dragonProtectedItem->setProtector(dragon); // DRAGON DEEZ NUTS ACROSS...
    }

    // Postprocessing: label chambers
    // This operates in-place with side effects on `newBoard`
    newBoard->chambers = labelChambers(floorTiles, newBoard);

    if (shouldGenerate) { // Random gen
        randomPopulateMap(newBoard, player, level == suitLevel);
    } else { // Passed in map
        if (stairLocation.first == 0 || stairLocation.second == 0) {
            throw std::invalid_argument{
                "invalid stair location in pre-determined map (" +
                std::to_string(stairLocation.first) + ", " + std::to_string(stairLocation.second) +
                ")"
            };
        }
        if (playerLocation.first == 0 || playerLocation.second == 0) {
            throw std::invalid_argument{
                "invalid player location in pre-determined map (" +
                std::to_string(playerLocation.first) + ", " +
                std::to_string(playerLocation.second) + ")"
            };
        }
        if (compassHoldingEnemies.size() == 0 &&
            (compassLocation.first == 0 || compassLocation.second == 0)) {
            throw std::invalid_argument{
                "cannot assign compass to enemy and location not given (" +
                std::to_string(compassLocation.first) + ", " +
                std::to_string(compassLocation.second) + ")"
            };
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

    if (level > 1) {
        player->log() << " to level " << Color::Green << level << Color::Reset;
    }
}

bool Game::playerMove(CardinalDirection dir) {
    // current player location
    auto loc = currentBoard->getPlayerLoc();

    // TODO: can I put an assertion here that player is not nullptr? It should always be defined
    // and things have gone very wrong if not
    Tile* curTile = currentBoard->at(loc);
    Player* player = curTile->player;
    Tile* newTile = currentBoard->inDirection(loc, dir);

    // My first co-op work term, you know, I was writing C code, and I used a `goto`, and my boss
    // looked at the code, and he said "what's this, what are you doing?", and you know, the `goto`
    // made sense there, and then he deleted it, and then he put it back, and he said "you're a
    // co-op, you're not allowed to use `goto`s, but I'm a senior guy so it's ok".
    // - Ondřej Lhoták
    if (newTile == nullptr) {
        // we used a goto in an assignment :)
        goto end;
    }

    // Check if we are going to walk onto a staircase
    if (newTile->location == currentBoard->stairLocation) {
        player->log() << " " << Color::BICyan << "PC" << Color::Reset << " moves down the "
                      << Color::UBlack << "stairs" << Color::Reset;

        nextLevel();
        render();

        return true;
    }

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
        player->log() << " " << Color::BICyan << "PC" << Color::Reset << " moves " << Color::UBlack
                      << directionToString(dir) << Color::Reset;
        auto newArea = currentBoard->getArea(currentBoard->getPlayerLoc());
        for (int dx = 0; dx <= 2; ++dx) {
            for (int dy = 0; dy <= 2; ++dy) {
                if (dx == 1 && dy == 1)
                    continue; // if both are 0, move on, don't let them not move

                // Needs to happen after enemy move?? (should enemies be able to attack player?)
                if (newArea[dy][dx] != nullptr) {
                    // remove enemy logging
                    // if (newArea[dy][dx]->enemy)
                    //     player->log() << " and sees a " << *newArea[dy][dx];
                    if (newArea[dy][dx]->item)
                        player->log() << " and sees an unknown " << Color::Purple << "potion"
                                      << Color::Reset;
                    else if (newArea[dy][dx]->treasure)
                        player->log()
                            << " and sees a " << Color::IYellow << "treasure" << Color::Reset;
                } else {
                    std::cout << "NULLPTR\n";
                }
            }
        }
        player->log() << ".";
    }

end:
    update();

    return player->getHealth() > 0;
}

bool Game::playerAttack(CardinalDirection dir) {
    // find the target location
    auto loc = currentBoard->getPlayerLoc();
    Tile* targetTile = currentBoard->inDirection(loc, dir);

    // if there is an enemy there, they are attacked
    Enemy* enemy = targetTile->enemy;
    if (enemy) {
        std::pair<int, int> attackStats = enemy->beAttacked(player->getAttack());
        player->log() << " " << Color::BICyan << "PC" << Color::Reset << " deals "
                      << attackStats.second << " damage to " << *targetTile << " ("
                      << enemy->getHealthColor() << std::max(0, attackStats.first) << Color::Reset
                      << " HP).";
        if (attackStats.first <= 0) {
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

bool Game::didWin() {
    return (level >= levelCount);
}

void Game::render() const {
    currentBoard->render(output);
    player->displayInfo(output);
    output << Color::BWhite << "Floor: " << Color::Reset << level << '\n' << std::endl;
}

int Game::getScore() const {
    return player->getScore();
}
