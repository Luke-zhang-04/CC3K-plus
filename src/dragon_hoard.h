#ifndef DRAGON_HOARD_H
#define DRAGON_HOARD_H
#include "dragon_protected.h"
#include "player.h"
#include "treasure.h"

class DragonHoard: public DragonProtected, Treasure {
    private:
        int amount = 6;

    public:
        bool onRetrieve(Player* player) override;
};

#endif // DRAGON_HOARD_H
