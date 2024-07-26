#ifndef RACES_H
#define RACES_H

#include "player.h"

class Dwarf: public Player {
    public:
        Dwarf(Board& board);
        void pickupGold(int amt) override;
};

class Elf: public Player {
    public:
        Elf(Board& board);
        void usePotion(int healthMod, int attackMod, int defenseMod) override;
};

class Human: public Player {
    public:
        Human(Board& board);
};

class Orc: public Player {
    public:
        Orc(Board& board);
        void pickupGold(int amt) override;
};

#endif
