#include "treasure.h"

#include "player.h"

char Treasure::getCharacter() {
    return 'G';
}

bool Treasure::onRetrieve(Player* player) {
    player->pickupGold(amount);
    return true;
}
