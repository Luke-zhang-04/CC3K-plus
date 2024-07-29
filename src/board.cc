#include "board.h"

#include "constants.h"
#include "enemy.h"
#include "game.h"
#include "random.h"
#include "tile.h"

#include <array>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

Tile* Board::at(size_t x, size_t y) const {
    return map.at(y).at(x);
}

Tile* Board::at(const std::pair<size_t, size_t>& coords) const {
    return map.at(coords.second).at(coords.first);
}

Tile* Board::inDirection(size_t x, size_t y, CardinalDirection dir) const {
    auto d = directionToDisplacement(dir);

    return map.at(y + d.second).at(x + d.first);
}

Tile* Board::inDirection(const std::pair<size_t, size_t>& loc, CardinalDirection dir) const {
    return inDirection(loc.first, loc.second, dir);
}

// use the given 2d vector of tiles, but populate stairLocation and playerLocation for easy
// updating
Board::Board(std::vector<std::vector<Tile*>> map, Game& game): game{game}, map{map} {
    bool foundStairs = false, foundPlayer = false;

    for (size_t y = 0; y < map.size(); y++) {
        std::vector<Tile*>& row = map[y];

        for (size_t x = 0; x < row.size(); x++) {
            Tile* cell = row[x];

            if (cell->mapTile == Symbol::Stairs) {
                stairLocation = {x, y};
                // show the floor until the stair is shown
                cell->mapTile = Symbol::FloorTile;
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

void Board::render(std::ostream& out, std::stringstream& logs) const {
    for (const std::vector<Tile*>& row : map) {
        for (const Tile* tile : row) {
            out << *tile;
        }
        out << '\n';
    }
    out << logs.str() << '\n';
    out << std::flush;
}

void Board::updateEnemies() {
    frame += 1;
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            Tile* t = map[y][x];
            if (t->enemy) {
                EnemyUpdateAction act = t->enemy->update(x, y, frame);

                if (act == EnemyUpdateAction::Attack) {
                    // if they don't miss, the player is attacked
                    if (randInt(0, 1) == 0) {
                        // don't worry if the player dies
                        std::pair<int, int> attackStats =
                            game.player->beAttacked(t->enemy->getPower());
                        game.player->log(
                            std::string(1, t->getCharacter()) + " deals " +
                            std::string(1, attackStats.second) + " damage to PC."
                        );
                    }
                }
                // otherwise, if they're doing anything they're moving
                else if (act != EnemyUpdateAction::NoAction) {
                    Tile* newTile = inDirection(x, y, static_cast<CardinalDirection>(act));
                    std::swap(t->enemy, newTile->enemy);
                }
            }
        }
    }
}

/** Returns nullptr if element is out of bounds */
template<typename T>
T optional2DIndex(
    const std::vector<std::vector<T>>& vec, size_t x, int xChange, size_t y, int yChange
) {
    if (y + yChange < 0 || y + yChange >= vec.size()) {
        return nullptr;
    }

    const std::vector<T>& row = vec[y + yChange];

    if (x + xChange < 0 || x + xChange >= row.size()) {
        return nullptr;
    }

    return row[x + xChange];
}

const std::array<const std::array<const Tile*, 3>, 3> Board::getArea(size_t x, size_t y) const {
    /* clang-format off */
    return {{
        {optional2DIndex(map, x, -1, y, -1), optional2DIndex(map, x, 0, y, -1), optional2DIndex(map, x, +1, y, -1)},
        {optional2DIndex(map, x, -1, y, 0),  optional2DIndex(map, x, 0, y, 0),  optional2DIndex(map, x, +1, y, 0) },
        {optional2DIndex(map, x, -1, y, +1), optional2DIndex(map, x, 0, y, +1), optional2DIndex(map, x, +1, y, +1)},
    }};
    /* clang-format on */
}

const std::array<const std::array<const Tile*, 3>, 3>
    Board::getArea(std::pair<size_t, size_t> coords) const {
    return Board::getArea(coords.first, coords.second);
}

const std::pair<size_t, size_t> Board::getPlayerLoc() {
    return playerLocation; // should return the value not a reference
}

const std::pair<size_t, size_t> Board::getStairLoc() {
    return stairLocation; // same as `getPlayerLoc()`
}

void Board::movePlayer(CardinalDirection dir) {
    auto d = directionToDisplacement(dir);

    playerLocation.first += d.first;
    playerLocation.second += d.second;
}
