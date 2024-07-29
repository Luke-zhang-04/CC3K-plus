#include "races.h"

#include "player.h"

#include <algorithm>

Dwarf::Dwarf(): Player{100, 20, 30} {}
Elf::Elf(): Player{140, 30, 10} {}
Human::Human(): Player{140, 20, 20} {}
Orc::Orc(): Player{180, 30, 25} {}

void Elf::usePotion(int healthMod, int attackMod, int defenseMod) {
    Player::usePotion(std::abs(healthMod), std::abs(attackMod), std::abs(defenseMod));
}

void Dwarf::pickupGold(int amt) {
    electrum += 4 * amt; // one gold is worth 2, which is worth 4 electrum
}

void Orc::pickupGold(int amt) {
    electrum += amt; // one gold is worth half, which is worth 1 electrum
}
