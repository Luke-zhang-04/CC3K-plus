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

#include <iostream>
#include <unordered_set>
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
            return new Tile{Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Vampire, board}};
        case Symbol::Werewolf:
            return new Tile{Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Werewolf, board}};
        case Symbol::Troll:
            return new Tile{Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Troll, board}};
        case Symbol::Goblin:
            return new Tile{Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Goblin, board}};
        case Symbol::Phoenix:
            return new Tile{Symbol::FloorTile, nullptr, nullptr, new Enemy{EnemyType::Phoenix, board}};

        case Symbol::Merchant: return new Tile{Symbol::FloorTile, nullptr, nullptr, new Merchant{board}};
        case Symbol::Dragon: return new Tile{Symbol::FloorTile, nullptr, nullptr, new Dragon{board}};

        case (char) InputMapNumbers::PotionRestoreHealth:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{10}};
        case (char) InputMapNumbers::PotionBoostAttack:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{0, 5}};
        case (char) InputMapNumbers::PotionBoostDefense:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{0, 0, 5}};
        case (char) InputMapNumbers::PotionPoisonHealth:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{-10}};
        case (char) InputMapNumbers::PotionWoundAttack:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{0, -5}};
        case (char) InputMapNumbers::PotionWoundDefense:
            return new Tile{Symbol::FloorTile, nullptr, new Potion{0, 0, -5}};
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

std::unordered_set<Tile*>& Game::traverseChamber(
    std::pair<size_t, size_t>&& tile,
    const Board* const newBoard,
    size_t currentChamberId,
    std::unordered_set<Tile*>& recursiveChambers
) {
    auto area = newBoard->getArea(tile);

    newBoard->at(tile)->chamberId = currentChamberId;
    recursiveChambers.insert(newBoard->at(tile));

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

std::vector<std::unordered_set<Tile*>> Game::labelChambers(
    std::vector<std::pair<size_t, size_t>>& floorTiles, const Board* const newBoard
) {
    size_t chamberIdCounter = 1;

    std::vector<std::unordered_set<Tile*>> chambers;

    for (auto& floorTile : floorTiles) {
        if (newBoard->at(floorTile)->chamberId == 0) {
            // We have an unlabeled boy, label him and his comrades
            chambers.emplace_back(std::unordered_set<Tile*>{});
            traverseChamber(
                std::move(floorTile), newBoard, chamberIdCounter++, chambers[chambers.size() - 1]
            );
        }
    }

    return chambers;
}

std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> Game::randomPopulateMap(
    std::vector<std::unordered_set<Tile*>>& chambers, Board* newBoard, Player* player
) {
    // if (floorTiles.size() <= 42) {
    //     throw std::length_error("board is not large enough to spawn everything");
    // }

    // shuffle(floorTiles);

    // newBoard->at(floorTiles[0])->player = player;

    // Spawn enemies
    // for (unsigned int8_t index = 2; index < 22; index++) {
    //     unsigned int8_t num = randInt(0, 18);
    //     Tile* curTile = newBoard->at(floorTiles[index]);

    //     if (0 <= num && 3 < num) {
    //         curTile->enemy = new Enemy{EnemyType::Vampire, *newBoard};
    //     } else if (3 <= num && num < 7) {
    //         curTile->enemy = new Enemy{EnemyType::Werewolf, *newBoard};
    //     } else if (7 <= num && num < 9) {
    //         curTile->enemy = new Enemy{EnemyType::Troll, *newBoard};
    //     } else if (9 <= num && num < 14) {
    //         curTile->enemy = new Enemy{EnemyType::Goblin, *newBoard};
    //     } else if (14 <= num && num < 16) {
    //         curTile->enemy = new Enemy{EnemyType::Merchant, *newBoard};
    //     } else {
    //         curTile->enemy = new Enemy{EnemyType::Phoenix, *newBoard};
    //     }
    // }

    // Pick compass bearer
    // for (unsigned int8_t index = 22; index < 32; index++) {}

    // return {floorTiles[0], floorTiles[1]};
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
    std::pair<size_t, size_t> compassLocation{0, 0};
    std::pair<size_t, size_t> stairLocation{0, 0};
    std::pair<size_t, size_t> playerLocation{0, 0};
    // TODO: there can actually be multiple dragons
    Dragon* dragon = nullptr;                          // NON OWNERSHIP
    DragonProtected* dragonProtectedItem = nullptr;    // NON OWNERSHIP
    std::vector<std::pair<size_t, size_t>> floorTiles; // Keep track of floor tiles for spawning
    std::vector<Enemy*> compassHoldingEnemies;         // Enemies that CAN hold a compass

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
        std::pair<size_t, size_t> currentLocation{row.size() - 1, tiles.size() - 1};

        if (input == Symbol::FloorTile) {
            floorTiles.push_back(currentLocation);
        }
        if (input == Symbol::Stairs) {
            stairLocation = currentLocation;
        }
        if (input == Symbol::Player) {
            playerLocation = currentLocation;
        }
        // "You can assume the file won't contain a compass." --(https://piazza.com/class/lvkzhr2nagi58v/post/294)
        if (input == Symbol::Compass) {
            compassLocation = currentLocation;
        }
        if (input == Symbol::BarrierSuit || input == (char)InputMapNumbers::TreasureDragonHoard) {
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
    chambers = labelChambers(floorTiles, newBoard); // If I try std::move I get -Wpessimizing-move?

    if (shouldGenerate) { // Random gen
        randomPopulateMap(chambers, newBoard, player);
    } else { // Passed in map
        if (stairLocation.first == 0 || stairLocation.second == 0) {
            throw std::invalid_argument("invalid stair location in pre-determined map");
        }
        if (playerLocation.first == 0 || playerLocation.second == 0) {
            throw std::invalid_argument("invalid player location in pre-determined map");
        }
        if (compassHoldingEnemies.size() == 0 && (compassLocation.first == 0 || compassLocation.second == 0)) {
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
    }

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
        if (enemy->beAttacked(player->getPower()) > 0) {
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

void Game::render() const {
    currentBoard->render(output);
    // TODO: info pannel
}
