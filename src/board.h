#ifndef BOARD_H
#define BOARD_H
#include "tile.h"

#include <array>
#include <ostream>
#include <vector>

class Game;

class Board {
    private:
        Game& game;
        bool stairsVisible = false;
        std::pair<size_t, size_t> playerLocation{0, 0};
        std::pair<size_t, size_t> stairLocation{0, 0};

        std::vector<std::vector<Tile*>> map;

    public:
        Board(std::vector<std::vector<Tile*>>, Game&);
        ~Board();

        bool merchantHostile();

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
