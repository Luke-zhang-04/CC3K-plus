#include "character.h"

#include <math.h>

Character::Character(int maxHealth, int attack, int defense):
    maxHealth{maxHealth}, health{maxHealth}, defense{defense}, attack{attack} {}

int Character::getPower() {
    return attack;
}

int Character::beAttacked(int attackPower) {
    health -= ceil((100 / (100 + defense))) * attackPower;

    return health;
}
