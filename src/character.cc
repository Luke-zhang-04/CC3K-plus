#include "character.h"

#include "constants.h"
#include "tile.h"

#include <cmath>
#include <string>
#include <utility>

Character::Character(int maxHealth, int attack, int defense):
    maxHealth{maxHealth}, health{maxHealth}, defense{defense}, attack{attack} {}

int Character::calculateDamage(int attackPower) const {
    return std::ceil((100.0 / (100 + getDefense())) * attackPower);
}

int Character::getAttack() const {
    return attack;
}

int Character::getDefense() const {
    return defense;
}

std::string Character::getHealthColor() const {
    if (health <= maxHealth / 4) {
        return Color::IRed;
    } else if (health <= maxHealth / 2) {
        return Color::IYellow;
    }

    return Color::IGreen;
}

std::pair<int, int> Character::beAttacked(int attackPower) {
    int damage = calculateDamage(attackPower);
    health -= damage;

    health = std::max(health, 0);

    return {health, damage};
}

bool Character::canMove(const Tile* t) const {
    char mapTile = t->mapTile;

    return t->empty() && mapTile == Symbol::FloorTile;
}

Character::~Character() {}
