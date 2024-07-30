#include "barrier_suit.h"

#include "constants.h"
#include "dragon.h"
#include "player.h"

BarrierSuit::BarrierSuit(Dragon* d): Retrievable{d} {}

char BarrierSuit::getCharacter() {
    return Symbol::BarrierSuit;
}

bool BarrierSuit::retrieved(Player* player) {
    player->pickupSuit();

    return true;
}
