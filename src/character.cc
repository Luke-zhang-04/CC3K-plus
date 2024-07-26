#include <math.h>

#include "character.h"

Character::Character(Board& board, int maxHealth, int attack, int defense):
    maxHealth{maxHealth}, board{board}, health{maxHealth}, defense{defense}, attack{attack} {}

int Character::getPower() {
    return attack;
}

int Character::beAttacked(int attackPower) {
    return ceil((100 / (100 + defense))) * attackPower;
}
