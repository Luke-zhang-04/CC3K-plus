#include "treasure.h"

char Treasure::getCharacter() {
    return 'G';
}

void onRetrieve(Player* player) {
    player->pickupGold(amount);
}

