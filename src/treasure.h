#ifndef TREASURE_H
#define TREASURE_H
#include "retrievable.h"

class Treasure: public Retrievable {
    private:
        int amount;

    protected:
        virtual bool retrieved(Player* player) override;

    public:
        Treasure(int amt = 0): amount{amt} {};
        char getCharacter() override;
};

#endif
