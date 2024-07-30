#include "player.h"

#include "constants.h"
#include "tile.h"

#include <algorithm>
#include <ostream>

Player::Player(int maxHealth, int attack, int defense): Character{maxHealth, attack, defense} {}

char Player::getCharacter() {
    return Symbol::Player;
}

void Player::usePotion(int healthBuff, int attackBuff, int defenseBuff) {
    attackMod += std::max(attackMod + attackBuff, 0);
    defenseMod = std::max(defenseMod + defenseBuff, 0);
    // don't gain more than max or less than 1
    health = std::max(std::min(health + healthBuff, maxHealth), 1);
}

void Player::pickupSuit() {
    suited = true;
}

void Player::reset() {
    attackMod = 0;
    defenseMod = 0;
    // TODO: anything else?
}

void Player::pickupGold(int amt) {
    electrum += amt * 2;
}

int Player::getHealth() const {
    return health;
}

int Player::getAttack() const {
    return attack + attackMod;
}

int Player::getDefense() const {
    return defense + defenseMod;
}

int Player::getElectrum() const {
    return electrum;
}

int Player::getScore() const {
    return electrum/2;
}

bool Player::canMove(const Tile* t) const {
    char mapTile = t->mapTile;
    return Character::canMove(t) || mapTile == Symbol::Door || mapTile == Symbol::Passage ||
           mapTile == Symbol::Stairs;
}

void Player::clearLog() {
    sysLog.str(std::string());
}

std::stringstream& Player::log() {
    return sysLog;
}

void Player::displayInfo(std::ostream& out) {
    // display gold
    out << Color::BWhite << "Gold: " << Color::IYellow << electrum / 2 << Color::Reset;
    if (electrum % 2 == 1)
        out << ".5";
    out << '\n';

    out << Color::BWhite << "HP: " << getHealthColor() << health << Color::Reset << '\n';
    out << Color::BWhite << "ATK: " << Color::Reset << attack + attackMod << '\n';
    out << Color::BWhite << "DEF: " << Color::Reset << defense + defenseMod << '\n';
    // logs begin with a space
    out << Color::BWhite << "Action:" << Color::Reset << sysLog.str() << "\n";
    clearLog();
}
