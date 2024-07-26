#if !defined(DRAGON_HOARD_H)
#define DRAGON_HOARD_H
#include "dragon_protected.h"
#include "treasure.h"
#include "player.h"

class DragonHoard: public DragonProtected, Treasure {
    private:
        int amount = 6;
    public:
        bool onRetrieve(Player* player) override;
};

#endif // DRAGON_HOARD_H
