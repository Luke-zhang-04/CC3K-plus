#include "barrier_suit.h"

#include "constants.h"
#include "player.h"

char BarrierSuit::getCharacter() {
    return Symbols::BarrierSuit;
}

bool BarrierSuit::onRetrieve(Player* player) {
    if (dragon == nullptr) {
        player->pickupSuit();
        return true;
    }

    return false;
}
