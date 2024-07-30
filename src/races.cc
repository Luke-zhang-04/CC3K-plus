#include "races.h"

#include "constants.h"
#include "player.h"

#include <algorithm>
#include <limits>

Dwarf::Dwarf(): Player{100, 20, 30} {}
Elf::Elf(): Player{140, 30, 10} {}
Human::Human(): Player{140, 20, 20} {}
Orc::Orc(): Player{180, 30, 25} {}
God::God(): Player{std::numeric_limits<int>::max(), std::numeric_limits<int>::max() - 1000, 1} {}

void Elf::usePotion(int healthMod, int attackMod, int defenseMod) {
    Player::usePotion(std::abs(healthMod), std::abs(attackMod), std::abs(defenseMod));
}

void Dwarf::pickupGold(int amt) {
    electrum += 4 * amt; // one gold is worth 2, which is worth 4 electrum
}

void Orc::pickupGold(int amt) {
    electrum += amt; // one gold is worth half, which is worth 1 electrum
}

void Dwarf::displayInfo(std::ostream& out) {
    out << Color::BWhite << "Race: " << Color::Reset << "Dwarf ";
    Player::displayInfo(out);
}

void Elf::displayInfo(std::ostream& out) {
    out << Color::BWhite << "Race: " << Color::Reset << "Elf ";
    Player::displayInfo(out);
}

void Human::displayInfo(std::ostream& out) {
    out << Color::BWhite << "Race: " << Color::Reset << "Human ";
    Player::displayInfo(out);
}

void Orc::displayInfo(std::ostream& out) {
    out << Color::BWhite << "Race: " << Color::Reset << "Orc ";
    Player::displayInfo(out);
}

void God::displayInfo(std::ostream& out) {
    out << Color::BWhite << "Race: " << Color::Reset << "God ";
    Player::displayInfo(out);
}
