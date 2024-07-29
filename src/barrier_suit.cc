#include "barrier_suit.h"

#include "constants.h"
#include "player.h"
#include "dragon.h"

BarrierSuit::BarrierSuit(Dragon* d): DragonProtected{d} {}

char BarrierSuit::getCharacter() {
    return Symbol::BarrierSuit;
}

bool BarrierSuit::onRetrieve(Player* player) {
    if (dragon == nullptr) {
        player->pickupSuit();
        return true;
    }

    return false;
}
