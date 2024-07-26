#include "potion.h"

char Potion::getCharacter() {
    return 'P';
}

char Potion::onInteract(Player* player) {
    player->usePotion(healthMod, attackMod, defenseMod);
}
