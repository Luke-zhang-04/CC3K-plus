#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "constants.h"
#include "player.h"

#include <iostream>

class Game {
        friend class Board;

    private:
        bool merchantHostile = false;
        std::ostream& output;
        std::istream& layoutInput; // Input for layout, endl represents end of layout
        Board* currentBoard = nullptr;
        Player* player;

        // update the board, then render the bottom bar. happens after a player action
        void update();
        void nextLevel(bool generate = true);

    public:
        Game(Player*, std::ostream&, std::istream&);
        ~Game();

        /** @returns boolean indicating if player is still alive or not */
        bool playerMove(CardinalDirection);
        /** @returns boolean indicating if player is still alive or not */
        bool playerAttack(CardinalDirection);
        /** @returns boolean indicating if player is still alive or not */
        bool playerPickup(CardinalDirection);
        bool isAlive();
};

#endif
