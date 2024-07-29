#include "constants.h"

#include <string>

bool isEnemy(char symbol) {
    return symbol == Symbol::Vampire || symbol == Symbol::Werewolf || symbol == Symbol::Troll ||
           symbol == Symbol::Goblin || symbol == Symbol::Merchant || symbol == Symbol::Dragon ||
           symbol == Symbol::Phoenix;
}

std::string symbolToColor(char symbol) {
    switch (symbol) {
        case Symbol::BarrierSuit:
        case Symbol::Compass: return Color::White; // TODO: these white/value

        case Symbol::Vampire: return Color::IRed;
        case Symbol::Werewolf: return Color::White;
        case Symbol::Troll:
        case Symbol::Goblin: return Color::Green;
        case Symbol::Merchant: return Color::Red;
        case Symbol::Dragon: return Color::BgRed;
        case Symbol::Phoenix: return Color::Yellow;

        case Symbol::Player: return Color::ICyan;

        case Symbol::Potion: return Color::Purple;

        case Symbol::Treasure: return Color::IYellow;

        case Symbol::Stairs:
        case Symbol::WallVert:
        case Symbol::WallHorz:
        case Symbol::Door:
        case Symbol::Passage: return Color::White;
        case Symbol::FloorTile: return Color::Black;
        case Symbol::Blank: return Color::Reset;

        default: return Color::Reset;
    }
}

CardinalDirection stringToDirection(std::string str) {
    if (str == "no") {
        return CardinalDirection::North;
    }
    if (str == "so") {
        return CardinalDirection::South;
    }
    if (str == "ea") {
        return CardinalDirection::East;
    }
    if (str == "we") {
        return CardinalDirection::West;
    }
    if (str == "ne") {
        return CardinalDirection::NorthEast;
    }
    if (str == "nw") {
        return CardinalDirection::NorthWest;
    }
    if (str == "se") {
        return CardinalDirection::SouthEast;
    }
    if (str == "sw") {
        return CardinalDirection::SouthWest;
    }

    throw std::invalid_argument("unknown direction " + str);
}
