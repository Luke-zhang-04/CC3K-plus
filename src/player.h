#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include "Treasure.h"

class Player: public Character {
    private:
        int attackMod;
        int defenseMod;
        bool suited;
        int gold;
    public:
        Player(Board& board, int maxHealth, int defence, int attack);
        char getCharacter();
        virtual void usePotion(int healthBuff, int attackBuff, int defenseBuff);
        virtual void pickupGold(Treasure gold);
        virtual void pickupSuit(Treasure suit);
        void reset();
};

#endif
