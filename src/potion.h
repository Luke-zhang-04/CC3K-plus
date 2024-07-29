#ifndef POTION_H
#define POTION_H
#include "interactible.h"

class Potion: public Interactible {
        int healthMod;
        int attackMod;
        int defenseMod;

    public:
        Potion(int h = 0, int a = 0, int d = 0): healthMod{h}, attackMod{a}, defenseMod{d} {};
        // Potion(const Potion&);
        char getCharacter() override;
        void onInteract(Player* player) override;
};

#endif
