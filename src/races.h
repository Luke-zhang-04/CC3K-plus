#ifndef RACES_H
#define RACES_H

#include "player.h"

#include <ostream>

class Dwarf: public Player {
    public:
        Dwarf();
        void pickupGold(int amt) override;
        void displayInfo(std::ostream&) override;
};

class Elf: public Player {
    public:
        Elf();
        void usePotion(int healthMod, int attackMod, int defenseMod) override;
        void displayInfo(std::ostream&) override;
};

class Human: public Player {
    public:
        Human();
        void displayInfo(std::ostream&) override;
        int getScore() const override;
};

class Orc: public Player {
    public:
        Orc();
        void pickupGold(int amt) override;
        void displayInfo(std::ostream&) override;
};

class God: public Player {
    public:
        God();
        // should not be affected by potions
        void usePotion(int healthMod, int attackMod, int defenseMod) override;
        void displayInfo(std::ostream&) override;
};

#endif
