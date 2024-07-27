#include "dragon_hoard.h"

#include "player.h"

bool DragonHoard::onRetrieve(Player* player) {
    if (dragon == nullptr) {
        player->pickupGold(amount);
        return true;
    }
    return false;
}
