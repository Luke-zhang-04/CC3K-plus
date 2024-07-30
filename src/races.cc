#include "races.h"

#include "constants.h"
#include "player.h"

#include <algorithm>
#include <limits>

Dwarf::Dwarf(): Player{100, 20, 30} {}
Elf::Elf(): Player{140, 30, 10} {}
Human::Human(): Player{140, 20, 20} {}
Orc::Orc(): Player{180, 30, 25} {}
God::God(): Player{std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), 1} {}

void Elf::usePotion(int healthMod, int attackMod, int defenseMod) {
    Player::usePotion(std::abs(healthMod), std::abs(attackMod), std::abs(defenseMod));
}

void Dwarf::pickupGold(int amt) {
    electrum += 4 * amt; // one gold is worth 2, which is worth 4 electrum
}

void Orc::pickupGold(int amt) {
    electrum += amt; // one gold is worth half, which is worth 1 electrum
}

void God::usePotion(int, int, int) {}

void Dwarf::displayInfo(std::ostream& out) {
    out << Color::BWhite << "Race: " << Color::Reset << Color::ItBlack << "Dwarf " << Color::Reset;
    Player::displayInfo(out);
}

void Elf::displayInfo(std::ostream& out) {
    out << Color::BWhite << "Race: " << Color::Reset << Color::ItWhite << "Elf " << Color::Reset;
    Player::displayInfo(out);
}

void Human::displayInfo(std::ostream& out) {
    out << Color::BWhite << "Race: " << Color::Reset << Color::ItYellow << "Human "
        << Color::Reset;
    Player::displayInfo(out);
}

int Human::getScore() const {
    // half again increase
    return Player::getScore() + Player::getScore()/2;
}

void Orc::displayInfo(std::ostream& out) {
    out << Color::BWhite << "Race: " << Color::Reset << Color::ItGreen << "Orc " << Color::Reset;
    Player::displayInfo(out);
}

void God::displayInfo(std::ostream& out) {
    out << Color::BWhite << "Race: " << Color::ItIYellow << "God " << Color::Reset;
    Player::displayInfo(out);
}
