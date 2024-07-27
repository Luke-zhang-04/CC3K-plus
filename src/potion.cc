#include "potion.h"

char Potion::getCharacter() {
    return 'P';
}

void Potion::onInteract(Player* player) {
    player->usePotion(healthMod, attackMod, defenseMod);
}
