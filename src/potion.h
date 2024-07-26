#if !defined(POTION_H)
#define POTION_H
#include "interactible.h"

class Potion: public Interactible {
    int healthMod;
    int attackMod;
    int defenseMod;

    public:
        Potion(int h, int a, int d):
            healthMod { h }, attackMod { a }, defenseMod { d };
        char getCharacter();
        void onInteract(Player* player);
};

#endif // POTION_H
