#include "dragon_hoard.h"

bool DragonHoard::onRetrieve(Player* player) {
    if (dragon == nullptr) {
        player->pickupGold(amount);
        return true;
    }
    return false;
}
