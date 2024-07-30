#include "player.h"

#include "constants.h"
#include "tile.h"

#include <algorithm>

Player::Player(int maxHealth, int attack, int defense): Character{maxHealth, attack, defense} {}

char Player::getCharacter() {
    return Symbol::Player;
}

void Player::usePotion(int healthBuff, int attackBuff, int defenseBuff) {
    attackMod += attackBuff;
    defenseMod += defenseBuff;
    health = std::max(health + healthBuff, maxHealth);
}

void Player::pickupSuit() {
    suited = true;
}

void Player::pickupGold(int amt) {
    electrum += amt * 2;
}

int Player::getHealth() {
    return health;
}

int Player::getAttack() {
    return attack;
}

int Player::getDefense() {
    return defense;
}

int Player::getElectrum() {
    return electrum;
}

bool Player::canMove(const Tile* t) const {
    char mapTile = t->mapTile;
    return Character::canMove(t) || mapTile == Symbol::Door || mapTile == Symbol::Passage ||
           mapTile == Symbol::Stairs;
}

void Player::log(std::string str) {
    if (sysLog.str().length() > 0)
        sysLog << " ";
    sysLog << str;
}

void Player::log(int i) {
    if (sysLog.str().length() > 0)
        sysLog << " ";
    sysLog << i;
}

void Player::clearLog() {
    sysLog.str(std::string());
}

std::stringstream& Player::getLog() {
    return sysLog;
}

void Player::displayInfo(ostream& out) {
    out << "Race: " << '\n';
}
