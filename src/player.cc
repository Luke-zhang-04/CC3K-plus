#include "player.h"

#include "constants.h"

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
