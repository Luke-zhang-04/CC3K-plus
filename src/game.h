#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "player.h"

#include <ostream>

enum class CardinalDirection {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
};

class Game {
        friend class Board;

    private:
        bool merchantHostile = false;
        std::ostream& output;
        Board* currentBoard = nullptr;
        Player* player;

        void nextLevel();

    public:
        Game(Player*, std::ostream&);
        ~Game();

        void render();
        void playerMove(CardinalDirection);
        void playerAttack(CardinalDirection);
        void playerPickup(CardinalDirection);
};

#endif
