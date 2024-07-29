#ifndef BOARD_H
#define BOARD_H
#include "constants.h"
#include "tile.h"

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

        Tile* at(size_t x, size_t y) const;
        Tile* at(const std::pair<size_t, size_t>&) const;
        Tile* inDirection(size_t x, size_t y, CardinalDirection) const;
        Tile* inDirection(const std::pair<size_t, size_t>&, CardinalDirection) const;

    public:
        Board(std::vector<std::vector<Tile*>>, Game&);
        ~Board();

        bool merchantHostile();

        void showStairs();

        // render all tiles to ostream
        void render(std::ostream&) const;

        // update all enemies
        void updateEnemies();

        /**
         * @brief Get area around x, y
         *
         * nullptr -> no tile here, out of bounds
         */
        const std::array<const std::array<const Tile*, 3>, 3> getArea(size_t x, size_t y) const;
        const std::array<const std::array<const Tile*, 3>, 3>
            getArea(std::pair<size_t, size_t>) const;

        const std::pair<size_t, size_t> getPlayerLoc();

        const std::pair<size_t, size_t> getStairLoc();
        void movePlayer(CardinalDirection);

};

#endif
