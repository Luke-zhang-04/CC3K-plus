#ifndef TREASURE_H
#define TREASURE_H
#include "retrievable.h"

class Treasure: public Retrievable {
    private:
        int amount;

    public:
        char getCharacter();
        void onRetrieve();
};

#endif
