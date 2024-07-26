#if !defined(INTERACTIBLE_H)
#define INTERACTIBLE_H

#include "player.h"

class Interactible {
    public:
        virtual char getCharacter() = 0;
        virtual char onInteract(Player* player) = 0;
};

#endif // INTERACTIBLE_H
