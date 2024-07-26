#include <math.h>

#include "character.h"

Character::Character(int maxHealth, Board& board, int defence, int attack):
    maxHealth{maxHealth}, board{board}, health{maxHealth}, defence{defence}, attack{attack} {}

int Character::getPower() {
    return attack;
}

int Character::beAttacked(int attackPower) {
    return ceil((100 / (100 + defence))) * attackPower;
}
