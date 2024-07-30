#include "potion.h"

#include "constants.h"

char Potion::getCharacter() {
    return Symbol::Potion;
}

void Potion::onInteract(Player* player) {
    player->log() << " PC uses " << type << ".";

    player->usePotion(healthMod, attackMod, defenseMod);
}
