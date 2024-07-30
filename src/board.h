#ifndef BOARD_H
#define BOARD_H
#include "constants.h"
#include "types.h"

#include <array>
#include <cstddef>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

using std::size_t;

class Game;
struct Tile;

class Board {
        friend class Game;

    private:
        Game& game;
        bool stairsVisible = false;
        coordPair playerLocation{0, 0};
        coordPair stairLocation{0, 0};

        std::vector<std::vector<Tile*>> map;

        /** Chambers is more like a vector to a map, except the keys are just sequential from 1 */
        std::vector<std::vector<Tile*>> chambers = {};

        unsigned int frame = 0;

        Tile* at(size_t x, size_t y) const;
        Tile* at(const coordPair&) const;
        Tile* inDirection(size_t x, size_t y, CardinalDirection) const;
        Tile* inDirection(const coordPair&, CardinalDirection) const;

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
        const std::array<const std::array<const Tile*, 3>, 3> getArea(coordPair) const;
        const std::array<const std::array<const Tile*, 3>, 3> getArea(Tile*) const;

        const coordPair getPlayerLoc();

        const coordPair getStairLoc();
        void movePlayer(CardinalDirection);
};

#endif
