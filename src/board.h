#ifndef BOARD_H
#define BOARD_H
#include "game.h"
#include "tile.h"

#include <array>
#include <ostream>
#include <vector>

class Board {
    private:
        Game& game;
        bool stairsVisible = false;
        std::pair<int, int> playerLocation;
        std::pair<int, int> stairLocation;

        std::vector<std::vector<Tile*>> map;

    public:
        Board(std::vector<std::vector<Tile*>>);

        bool merchantHostile() {
            return game.merchantHostile;
        }

        void showStairs() {
            stairsVisible = true;
        }

        void render(std::ostream&);

        /**
         * @brief Get area around x, y
         *
         * nullptr -> no tile here, out of bounds
         */
        const std::array<const std::array<const Tile*, 3>, 3> getArea(int x, int y);
};

#endif
