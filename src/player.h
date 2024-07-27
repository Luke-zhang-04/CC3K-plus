#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Board;

class Player: public Character {
    protected:
        bool suited;
        int attackMod;
        int defenseMod;
        int gold;

    public:
        Player(Board& board, int maxHealth, int defence, int attack);
        char getCharacter();
        virtual void usePotion(int healthBuff, int attackBuff, int defenseBuff);
        virtual void pickupGold(int amt);
        virtual void pickupSuit();
        void reset();
};

#endif
