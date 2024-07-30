#ifndef POTION_H
#define POTION_H
#include "interactible.h"

#include <string>

class Potion: public Interactible {
    private:
        std::string type;
        int healthMod;
        int attackMod;
        int defenseMod;

    public:
        Potion(std::string t, int h = 0, int a = 0, int d = 0):
            type{t}, healthMod{h}, attackMod{a}, defenseMod{d} {};
        char getCharacter() override;
        void onInteract(Player* player) override;
};

#endif
