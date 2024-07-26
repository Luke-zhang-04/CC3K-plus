#include "barrier_suit.h"

char BarrierSuit::getCharacter() {
    return 'B';
}

bool BarrierSuit::onRetrieve(Player* player) {
    if (dragon == nullptr) {
        player->pickupSuit();
        return true;
    }

    return false;
}
