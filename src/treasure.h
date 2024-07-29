#ifndef TREASURE_H
#define TREASURE_H
#include "retrievable.h"

class Treasure: public Retrievable {
    private:
        int amount;

    public:
        Treasure(int amt = 0): amount{amt} {};
        char getCharacter() override;
        virtual bool onRetrieve(Player* player) override;
};

#endif
