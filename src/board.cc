#include "board.h"

#include "constants.h"
#include "game.h"
#include "tile.h"
#include "enemy.h"

#include <array>
#include <ostream>
#include <vector>
#include <stdlib.h> // rand
#include <string>

Tile* Board::inDirection(int x, int y, CardinalDirection dir) {
    int dx = (int) dir % 3 - 1;
    int dy = (int) dir / 3 - 1;

    return map[y + dy][x + dx];
}

Tile* Board::inDirection(std::pair<int, int> loc, CardinalDirection dir) {
    return inDirection(loc.first, loc.second, dir);
}

// use the given 2d vector of tiles, but populate stairLocation and playerLocation for easy updating
Board::Board(std::vector<std::vector<Tile*>> map, Game& game): map{map}, game{game} {
    bool foundStairs = false, foundPlayer = false;

    for (size_t y = 0; y < map.size(); y++) {
        std::vector<Tile*>& row = map[y];

        for (size_t x = 0; x < row.size(); x++) {
            Tile* cell = row[x];

            if (cell->mapTile == Symbol::Stairs) {
                stairLocation = {x, y};
                // show the floor until the stair is shown
                cell->mapTile == Symbol::FloorTile;
                foundStairs = true;
            }
            if (cell->player != nullptr) {
                playerLocation = {x, y};
                foundPlayer = true;
            }

            if (foundPlayer && foundStairs) {
                break;
            }
        }
    }
}

Board::~Board() {
    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[y].size(); x++) {
            delete map[y][x];
        }
    }
}

bool Board::merchantHostile() {
    return game.merchantHostile;
}

void Board::showStairs() {
    stairsVisible = true;
    // replace the map tile of the stair with the appropriate symbol
    map[stairLocation.second][stairLocation.first]->mapTile = Symbol::Stairs;
}

void Board::render(std::ostream& out) {
    for (const std::vector<Tile*>& row : map) {
        for (const Tile* tile : row) {
            out << tile;
        }
        out << std::endl; // TODO: Is it ok to flush this often?
    }
}

void Board::updateEnemies() {
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            Tile* t = map[y][x];
            if (t->enemy) {
                EnemyUpdateAction act = t->enemy->update(x, y);

                if (act == EnemyUpdateAction::Attack) {
                    // if they don't miss, the player is attacked
                    if (rand() % 2 == 0) {
                        // don't worry if the player dies
                        game.player->beAttacked(t->enemy->getPower());
                    }
                }
                // otherwise, if they're doing anything they're moving
                else if (act != EnemyUpdateAction::NoAction) {
                    // They've just checked that it's a legal move, so move them immediately
                    // TODO: Use inDirection
                }
            }
        }
    }
}

/** Returns nullptr if element is out of bounds */
template<typename T>
T optional2DIndex(std::vector<std::vector<T>>& vec, int x, int y) {
    if (y < 0 || y >= vec.size()) {
        return nullptr
    }

    std::vector<T*>& row = vec[y];

    if (x < 0 || x >= row.size()) {
        return nullptr;
    }

    return row[x];
}

const std::array<const std::array<const Tile*, 3>, 3> Board::getArea(int x, int y) {
    /* clang-format off */
    return {{
        {optional2DIndex(map, x - 1, y - 1), optional2DIndex(map, x, y - 1), optional2DIndex(map, x + 1, y - 1)},
        {optional2DIndex(map, x - 1, y),     optional2DIndex(map, x, y),     optional2DIndex(map, x + 1, y)    },
        {optional2DIndex(map, x - 1, y + 1), optional2DIndex(map, x, y + 1), optional2DIndex(map, x + 1, y + 1)},
    }};
    /* clang-format on */
}

const std::pair<int, int> Board::getPlayerLoc() {
    return playerLocation; // should return the value not a reference
}
