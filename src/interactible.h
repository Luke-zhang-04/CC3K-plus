#ifndef INTERACTIBLE_H
#define INTERACTIBLE_H

#include "player.h"

class Interactible {
    public:
        virtual char getCharacter() = 0;
        virtual void onInteract(Player* player) = 0;
};

#endif
