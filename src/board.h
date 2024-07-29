#ifndef BOARD_H
#define BOARD_H
#include "tile.h"
#include "constants.h"

#include <array>
#include <ostream>
#include <string>
#include <vector>

class Game;

class Board {
    friend class Game;

    private:
        Game& game;
        bool stairsVisible = false;
        std::pair<size_t, size_t> playerLocation{0, 0};
        std::pair<size_t, size_t> stairLocation{0, 0};

        std::vector<std::vector<Tile*>> map;

        Tile* inDirection(int x, int y, CardinalDirection);
        Tile* inDirection(std::pair<int, int>, CardinalDirection);

    public:
        Board(std::vector<std::vector<Tile*>>, Game&);
        ~Board();

        bool merchantHostile();

        void showStairs();

        // render all tiles to ostream
        void render(std::ostream&);

        // update all enemies
        void updateEnemies();

        /**
         * @brief Get area around x, y
         *
         * nullptr -> no tile here, out of bounds
         */
        const std::array<const std::array<const Tile*, 3>, 3> getArea(int x, int y);

        const std::pair<int, int> getPlayerLoc();
};

#endif
