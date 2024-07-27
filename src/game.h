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

    public:
        Game(Player*, std::ostream&);

        Game(const Game& other);
        Game(Game&& other);
        Game& operator=(const Game& other);
        Game& operator=(Game&& other);
        ~Game();

        void render();
        void playerMove(CardinalDirection);
        void playerAttack(CardinalDirection);
        void playerPickup(CardinalDirection);
};

#endif
