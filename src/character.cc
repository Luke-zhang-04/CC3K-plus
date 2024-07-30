#include "character.h"

#include "tile.h"

#include <cmath>
#include <utility>

Character::Character(int maxHealth, int attack, int defense):
    maxHealth{maxHealth}, health{maxHealth}, defense{defense}, attack{attack} {}

int Character::getAttack() {
    return attack;
}

int Character::getDefense() {
    return defense;
}

std::pair<int, int> Character::beAttacked(int attackPower) {
    int attackCalc = std::ceil((100.0 / (100 + getDefense())) * attackPower);
    health -= attackCalc;

    return {health, attackCalc};
}

bool Character::canMove(const Tile* t) const {
    char mapTile = t->mapTile;

    return t->empty() && mapTile == Symbol::FloorTile;
}

Character::~Character() {}
