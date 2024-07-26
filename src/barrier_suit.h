#if !defined(BARRIER_SUIT)
#define BARRIER_SUIT
#include "retrievable.h"
#include "dragon_protected.h"
#include "player.h"

class BarrierSuit: public Retrievable, DragonProtected {
    public:
        char getCharacter() override;
        bool onRetrieve(Player* player) override;
};

#endif // BARRIER_SUIT
