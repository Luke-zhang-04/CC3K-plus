#if !defined(INTERACTIBLE_H)
#define INTERACTIBLE_H

#include "player.h"

class Interactible {
    public:
        char getCharacter() = 0;
        char onInteract(Player* player) = 0;
}

#endif // INTERACTIBLE_H
