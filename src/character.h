#ifndef CHARACTER_H
#define CHARACTER_H
#include "board.h"

class Character {
    protected:
        Board& board;
        int maxHealth, health, defense, attack;

    public:
        Character(Board& board, int maxHealth, int defense, int attack);

        /** Character to be displayed on board */
        virtual char getCharacter() = 0;

        /** Attacking power */
        int getPower();

        int beAttacked(int attackPower);
};

#endif
