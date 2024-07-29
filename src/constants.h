#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>

using std::string;

namespace Symbol {
    const char BarrierSuit = 'B';
    const char Compass = 'C';

    const char Vampire = 'V';
    const char Werewolf = 'W';
    const char Troll = 'T';
    const char Goblin = 'N';
    const char Merchant = 'M';
    const char Dragon = 'D';
    const char Phoenix = 'X';

    const char Player = '@';

    const char Potion = 'P';

    const char Treasure = 'G';

    const char Stairs = '\\';
    const char WallVert = '|';
    const char WallHorz = '-';
    const char Door = '+';
    const char Passage = '#';
    const char FloorTile = '.';
    const char Blank = ' ';
}; // namespace Symbol

enum class CardinalDirection {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
};

enum class InputMapNumbers {
    //  0 - RH, 1 - BA, 2 - BD, 3 - PH, 4 - WA, 5 - WD, 6 - normal gold pile, 7 - small hoard, 8 -
    //  merchant hoard, 9 - dragon hoard.
    PotionRestoreHealth = '0',
    PotionBoostAttack,
    PotionBoostDefense,
    PotionPoisonHealth,
    PotionWoundAttack,
    PotionWoundDefense,
    TreasureSmallGoldPile,
    TreasureSmallHoard,
    TreasureMerchantHoard,
    TreasureDragonHoard,
};

namespace Color {
    // Source: https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
    /*
     * This is free and unencumbered software released into the public domain.
     *
     * For more information, please refer to <https://unlicense.org>
     */

    // Regular Color
    const string Black = "\e[0;30m";
    const string Red = "\e[0;31m";
    const string Green = "\e[0;32m";
    const string Yellow = "\e[0;33m";
    const string Blue = "\e[0;34m";
    const string Purple = "\e[0;35m";
    const string Cyan = "\e[0;36m";
    const string White = "\e[0;37m";

    // Bold
    const string BBlack = "\e[1;30m";
    const string BRed = "\e[1;31m";
    const string BGreen = "\e[1;32m";
    const string BYellow = "\e[1;33m";
    const string BBlue = "\e[1;34m";
    const string BPurple = "\e[1;35m";
    const string BCyan = "\e[1;36m";
    const string BWhite = "\e[1;37m";

    // Underline
    const string UBlack = "\e[4;30m";
    const string URed = "\e[4;31m";
    const string UGreen = "\e[4;32m";
    const string UYellow = "\e[4;33m";
    const string UBlue = "\e[4;34m";
    const string UPurple = "\e[4;35m";
    const string UCyan = "\e[4;36m";
    const string UWhite = "\e[4;37m";

    // Background
    const string BgBlack = "\e[40m";
    const string BgRed = "\e[41m";
    const string BgGreen = "\e[42m";
    const string BgYellow = "\e[43m";
    const string BgBlue = "\e[44m";
    const string BgPurple = "\e[45m";
    const string BgCyan = "\e[46m";
    const string BgWhite = "\e[47m";

    // High Intensity
    const string IBlack = "\e[0;90m";
    const string IRed = "\e[0;91m";
    const string IGreen = "\e[0;92m";
    const string IYellow = "\e[0;93m";
    const string IBlue = "\e[0;94m";
    const string IPurple = "\e[0;95m";
    const string ICyan = "\e[0;96m";
    const string IWhite = "\e[0;97m";

    // Bold High Intensity
    const string BIBlack = "\e[1;90m";
    const string BIRed = "\e[1;91m";
    const string BIGreen = "\e[1;92m";
    const string BIYellow = "\e[1;93m";
    const string BIBlue = "\e[1;94m";
    const string BIPurple = "\e[1;95m";
    const string BICyan = "\e[1;96m";
    const string BIWhite = "\e[1;97m";

    // High Intensity backgrounds
    const string IBgBlack = "\e[0;100m";
    const string IBgRed = "\e[0;101m";
    const string IBgGreen = "\e[0;102m";
    const string IBgYellow = "\e[0;103m";
    const string IBgBlue = "\e[0;104m";
    const string IBgPurple = "\e[0;105m";
    const string IBgCyan = "\e[0;106m";
    const string IBgWhite = "\e[0;107m";

    // Reset
    const string Reset = "\e[0m";
} // namespace Color

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
}

#endif
