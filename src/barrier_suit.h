#ifndef BARRIER_SUIT_H
#define BARRIER_SUIT_H
#include "retrievable.h"

class Player;
class Dragon;

class BarrierSuit: public Retrievable {
    protected:
        bool retrieved(Player*) override;

    public:
        BarrierSuit(Dragon* d);
        char getCharacter() override;
};

#endif // BARRIER_SUIT
