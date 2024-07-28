#include "potion.h"

#include "constants.h"

char Potion::getCharacter() {
    return Symbols::Potion;
}

void Potion::onInteract(Player* player) {
    player->usePotion(healthMod, attackMod, defenseMod);
}
