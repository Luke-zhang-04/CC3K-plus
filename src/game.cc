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
#include "retrievable.h"
#include "tile.h"
#include "treasure.h"

#include <iostream>
#include <vector>

Game::Game(Player* player, std::ostream& output, std::istream& layoutInput):
    output{output}, layoutInput{layoutInput}, player{player} {}

Game::~Game() {
    delete currentBoard;
    delete player;
}

void Game::update() {
    currentBoard->updateEnemies();
    currentBoard->render(output);
    // todo: render bottom bar
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

        case Symbol::Player: return new Tile{character, nullptr, nullptr, nullptr, player};

        case Symbol::BarrierSuit: return new Tile{character, new BarrierSuit{nullptr}};
        case Symbol::Compass: return new Tile{character, new Compass{board}};

        case Symbol::Vampire:
            return new Tile{character, nullptr, nullptr, new Enemy{EnemyType::Vampire, board}};
        case Symbol::Werewolf:
            return new Tile{character, nullptr, nullptr, new Enemy{EnemyType::Werewolf, board}};
        case Symbol::Troll:
            return new Tile{character, nullptr, nullptr, new Enemy{EnemyType::Troll, board}};
        case Symbol::Goblin:
            return new Tile{character, nullptr, nullptr, new Enemy{EnemyType::Goblin, board}};
        case Symbol::Phoenix:
            return new Tile{character, nullptr, nullptr, new Enemy{EnemyType::Phoenix, board}};

        case Symbol::Merchant: return new Tile{character, nullptr, nullptr, new Merchant{board}};
        case Symbol::Dragon: return new Tile{character, nullptr, nullptr, new Dragon{board}};

        case (char) InputMapNumbers::PotionRestoreHealth:
            return new Tile{character, nullptr, new Potion{10}};
        case (char) InputMapNumbers::PotionBoostAttack:
            return new Tile{character, nullptr, new Potion{0, 5}};
        case (char) InputMapNumbers::PotionBoostDefense:
            return new Tile{character, nullptr, new Potion{0, 0, 5}};
        case (char) InputMapNumbers::PotionPoisonHealth:
            return new Tile{character, nullptr, new Potion{-10}};
        case (char) InputMapNumbers::PotionWoundAttack:
            return new Tile{character, nullptr, new Potion{0, -5}};
        case (char) InputMapNumbers::PotionWoundDefense:
            return new Tile{character, nullptr, new Potion{0, 0, -5}};
        case (char) InputMapNumbers::TreasureSmallGoldPile:
            return new Tile{character, new Treasure{1}};
        case (char) InputMapNumbers::TreasureSmallHoard:
            return new Tile{character, new Treasure{2}};
        case (char) InputMapNumbers::TreasureMerchantHoard:
            return new Tile{character, new Treasure{4}};
        case (char) InputMapNumbers::TreasureDragonHoard:
            return new Tile{character, new Treasure{6}};

        default:
            throw std::invalid_argument("unknown map tile character " + std::string(1, character));
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
 *
 * @param generate - if game items should be generated or not
 */
void Game::nextLevel(bool generate) {
    std::pair<size_t, size_t> compassLocation{0, 0};
    std::pair<size_t, size_t> stairLocation{0, 0};
    std::pair<size_t, size_t> playerLocation{0, 0};
    std::pair<size_t, size_t> dragonLocation{0, 0};
    std::pair<size_t, size_t> dragonProtectedItemLocation{0, 0};
    std::vector<std::pair<size_t, size_t>> floorTiles; // Keep track of floor tiles for spawning
    std::vector<std::pair<size_t, size_t>> compassHolders; // Enemies that CAN hold a compass

    Board* newBoard = new Board{std::vector<std::vector<Tile*>>(1, std::vector<Tile*>{}), *this};

    char character;
    bool isEnd = false; // Tracks if a line has only | and -, but not the first line

    while (layoutInput >> character) {
        std::vector<std::vector<Tile*>>& tiles = newBoard->map;
        std::vector<Tile*>& row = tiles[tiles.size() - 1];

        if (character == '\n') {
            if (isEnd) { // End of floor
                break;
            }

            tiles.emplace_back(std::vector<Tile*>{});
            isEnd = true;
        }
        if (character != Symbol::WallHorz || character != Symbol::WallVert) {
            isEnd = false;
        }

        row.emplace_back(getTileFromChar(character, player, *newBoard));
        std::pair<size_t, size_t> currentLocation{row.size() - 1, tiles.size() - 1};

        if (character == Symbol::FloorTile) {
            floorTiles.push_back(currentLocation);
        }
        if (character == Symbol::Stairs) {
            stairLocation = currentLocation;
        }
        if (character == Symbol::Player) {
            playerLocation = currentLocation;
        }
        if (character == Symbol::Compass) {
            compassLocation = currentLocation;
        }
        if (character == Symbol::BarrierSuit || character == Symbol::Compass) {
            dragonProtectedItemLocation = currentLocation;
        }
        if (isEnemy(character) && character != Symbol::Merchant && character != Symbol::Dragon) {
            compassHolders.push_back(currentLocation);
        }
    }

    // Postprocessing: connect dragon to protected
    // Random gen

    delete currentBoard;
    currentBoard = newBoard;
}

bool Game::playerMove(CardinalDirection dir) {
    // current player location
    auto loc = currentBoard->getPlayerLoc();

    // TODO: can I put an assertion here that player is not nullptr? It should always be defined
    // and things have gone very wrong if not
    Tile* curTile = currentBoard->map[loc.second][loc.first];
    Player* player = curTile->player;
    Tile* newTile = currentBoard->inDirection(loc.first, loc.second, dir);

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
    if (newTile->movable()) {
        // move the player to the open location
        std::swap(curTile->player, newTile->player);
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
