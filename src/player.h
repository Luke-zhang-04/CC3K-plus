#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

#include <iostream>
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
        void clearLog();

    public:
        Player(int maxHealth, int attack, int defense);

        char getCharacter() override;
        virtual void usePotion(int healthBuff, int attackBuff, int defenseBuff);
        virtual void pickupGold(int amt);
        virtual void pickupSuit();
        void reset();

        int getHealth();
        int getAttack() override;
        int getDefense() override;
        int getElectrum();

        bool canMove(const Tile*) const override;

        std::stringstream& log(); // remove

        virtual void displayInfo(std::ostream&);
};

#endif
