#ifndef BARRIER_SUIT_H
#define BARRIER_SUIT_H
#include "dragon_protected.h"
#include "retrievable.h"

class Player;
class Dragon;

class BarrierSuit: public Retrievable, DragonProtected {
    public:
        BarrierSuit(Dragon* d);
        char getCharacter() override;
        bool onRetrieve(Player* player) override;
};

#endif // BARRIER_SUIT
