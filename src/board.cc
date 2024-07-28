#include "board.h"

#include "constants.h"
#include "game.h"
#include "tile.h"

#include <array>
#include <ostream>
#include <vector>

Board::Board(std::vector<std::vector<Tile*>> map, Game& game): map{map}, game{game} {
    bool foundStairs = false, foundPlayer = false;

    for (size_t y = 0; y < map.size(); y++) {
        std::vector<Tile*>& row = map[y];

        for (size_t x = 0; x < row.size(); x++) {
            Tile* cell = row[x];

            if (cell->mapTile == Symbols::Stairs) {
                stairLocation = {x, y};
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

void Board::render(std::ostream& out) {
    for (const std::vector<Tile*>& row : map) {
        for (const Tile* tile : row) {
            out << tile;
        }
        out << std::endl; // TODO: Is it ok to flush this often?
    }
}

/** Returns nullptr if element is out of bounds */
template<typename T>
T optional2DIndex(std::vector<std::vector<T>>& vec, int x, int y) {
    if (x < 0 || x >= vec.size()) {
        return nullptr
    }

    std::vector<T*>& row = vec[x];

    if (y < 0 || y >= row.size()) {
        return nullptr;
    }

    return row[y];
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
