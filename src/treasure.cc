#include "treasure.h"

#include "constants.h"
#include "player.h"

char Treasure::getCharacter() {
    return Symbol::Treasure;
}

bool Treasure::retrieved(Player* player) {
    player->pickupGold(amount);
    return true;
}
