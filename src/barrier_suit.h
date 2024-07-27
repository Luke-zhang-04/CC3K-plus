#ifndef BARRIER_SUIT_H
#define BARRIER_SUIT_H
#include "dragon_protected.h"
#include "player.h"
#include "retrievable.h"

class BarrierSuit: public Retrievable, DragonProtected {
    public:
        char getCharacter() override;
        bool onRetrieve(Player* player) override;
};

#endif // BARRIER_SUIT
