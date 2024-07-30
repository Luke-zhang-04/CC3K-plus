#include "constants.h"

#include <stdexcept>
#include <string>
#include <utility>

bool isEnemy(char symbol) {
    return symbol == Symbol::Vampire || symbol == Symbol::Werewolf || symbol == Symbol::Troll ||
           symbol == Symbol::Goblin || symbol == Symbol::Merchant || symbol == Symbol::Dragon ||
           symbol == Symbol::Phoenix;
}

std::string symbolToColor(char symbol) {
    switch (symbol) {
        case Symbol::BarrierSuit:
        case Symbol::Compass:
            return Color::White; // TODO: these white/values may not show up if light mode?

        case Symbol::Vampire: return Color::IRed;
        case Symbol::Werewolf: return Color::White;
        case Symbol::Troll:
        case Symbol::Goblin: return Color::Green;
        case Symbol::Merchant: return Color::Red;
        case Symbol::Dragon: return Color::BgRed;
        case Symbol::Phoenix: return Color::Yellow;

        case Symbol::Player: return Color::BICyan;

        case Symbol::Potion: return Color::Purple;

        case Symbol::Treasure: return Color::IYellow;

        case Symbol::WallVert:
        case Symbol::WallHorz:
        case Symbol::Stairs:
        case Symbol::Door:
        case Symbol::Passage: return Color::White;
        case Symbol::FloorTile: return Color::Black;
        case Symbol::Blank: return Color::Reset;

        default: return Color::Reset;
    }
}

CardinalDirection stringToDirection(std::string str) {
    if (str == "no" || str == ArrowKey::StrKeyUp) {
        return CardinalDirection::North;
    }
    if (str == "so" || str == ArrowKey::StrKeyDown) {
        return CardinalDirection::South;
    }
    if (str == "ea" || str == ArrowKey::StrKeyRight) {
        return CardinalDirection::East;
    }
    if (str == "we" || str == ArrowKey::StrKeyLeft) {
        return CardinalDirection::West;
    }
    if (str == "ne" || str == ArrowKey::StrKeyUpRight || str == ArrowKey::StrKeyRightUp) {
        return CardinalDirection::NorthEast;
    }
    if (str == "nw" || str == ArrowKey::StrKeyUpLeft || str == ArrowKey::StrKeyLeftUp) {
        return CardinalDirection::NorthWest;
    }
    if (str == "se" || str == ArrowKey::StrKeyDownRight || str == ArrowKey::StrKeyRightDown) {
        return CardinalDirection::SouthEast;
    }
    if (str == "sw" || str == ArrowKey::StrKeyDownLeft || str == ArrowKey::StrKeyLeftDown) {
        return CardinalDirection::SouthWest;
    }

    throw std::invalid_argument("unknown direction " + str);
}

std::string directionToString(CardinalDirection c) {
    switch (c) {
        case CardinalDirection::NorthWest: return "north west";
        case CardinalDirection::North: return "north";
        case CardinalDirection::NorthEast: return "north east";
        case CardinalDirection::West: return "west";
        case CardinalDirection::Center: return "center";
        case CardinalDirection::East: return "east";
        case CardinalDirection::SouthWest: return "south west";
        case CardinalDirection::South: return "south";
        case CardinalDirection::SouthEast: return "south east";
        default: return "";
    }
}

std::pair<int, int> directionToDisplacement(CardinalDirection dir) {
    return {((int) dir) % 3 - 1, ((int) dir) / 3 - 1};
}
