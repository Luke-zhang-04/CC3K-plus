#include "character.h"

#include "tile.h"

#include <math.h>
#include <utility>

Character::Character(int maxHealth, int attack, int defense):
    maxHealth{maxHealth}, health{maxHealth}, defense{defense}, attack{attack} {}

int Character::getPower() {
    return attack;
}

std::pair<int, int> Character::beAttacked(int attackPower) {
    health -= ceil((100 / (100 + defense))) * attackPower;

    return {health, ceil((100 / (100 + defense))) * attackPower};
}

bool Character::canMove(const Tile* t) const {
    char mapTile = t->mapTile;

    return t->empty() && mapTile == Symbol::FloorTile;
}

Character::~Character() {}
