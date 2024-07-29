#ifndef POTION_H
#define POTION_H
#include "interactible.h"

class Potion: public Interactible {
        int healthMod;
        int attackMod;
        int defenseMod;

    public:
        Potion(int h = 0, int a = 0, int d = 0): healthMod{h}, attackMod{a}, defenseMod{d} {};
        char getCharacter();
        void onInteract(Player* player);
};

#endif
