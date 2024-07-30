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
    health -= std::ceil((100.0 / (100 + getDefense())) * attackPower);

    return {health, std::ceil((100.0 / (100 + getDefense())) * attackPower)};
}

bool Character::canMove(const Tile* t) const {
    char mapTile = t->mapTile;

    return t->empty() && mapTile == Symbol::FloorTile;
}

Character::~Character() {}
