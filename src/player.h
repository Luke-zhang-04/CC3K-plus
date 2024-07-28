#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Board;

class Player: public Character {
    protected:
        bool suited = false;
        int attackMod = 0;
        int defenseMod = 0;
        int gold = 0;

    public:
        Player(Board& board, int maxHealth, int defence, int attack);

        char getCharacter();
        virtual void usePotion(int healthBuff, int attackBuff, int defenseBuff);
        virtual void pickupGold(int amt);
        virtual void pickupSuit();
        void reset();
};

#endif
