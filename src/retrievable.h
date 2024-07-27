#ifndef RETRIEVABLE_H
#define RETRIEVABLE_H

#include "player.h"

class Retrievable {
    public:
        virtual char getCharacter() = 0;
        virtual bool onRetrieve(Player* player) = 0;
};

#endif // RETRIEVABLE_H
