#include "treasure.h"

#include "constants.h"
#include "player.h"

char Treasure::getCharacter() {
    return Symbols::Treasure;
}

bool Treasure::onRetrieve(Player* player) {
    player->pickupGold(amount);
    return true;
}
