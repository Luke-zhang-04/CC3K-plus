#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

#include <sstream>

class Board;
struct Tile;

class Player: public Character {
    protected:
        bool suited = false;
        int attackMod = 0;
        int defenseMod = 0;
        int electrum = 0; // half gold increments
        std::stringstream sysLog;

    public:
        Player(int maxHealth, int defense, int attack);

        char getCharacter() override;
        virtual void usePotion(int healthBuff, int attackBuff, int defenseBuff);
        virtual void pickupGold(int amt);
        virtual void pickupSuit();
        void reset();

        int getHealth();
        int getAttack();
        int getDefense();
        int getElectrum();

        bool canMove(const Tile*) const override;

        void log(std::string);
        std::stringstream& getLog();
};

#endif
