#ifndef RACES_H
#define RACES_H

#include "player.h"

class Dwarf: public Player {
    public:
        Dwarf();
        void pickupGold(int amt) override;
};

class Elf: public Player {
    public:
        Elf();
        void usePotion(int healthMod, int attackMod, int defenseMod) override;
};

class Human: public Player {
    public:
        Human();
};

class Orc: public Player {
    public:
        Orc();
        void pickupGold(int amt) override;
};

#endif
