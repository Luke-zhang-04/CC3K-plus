#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>

using std::string;

namespace Symbols {
    const char BarrierSuit = 'B';
    const char Compass = 'C';

    const char Vampire = 'V';
    const char Werewolf = 'W';
    const char Troll = 'T';
    const char Goblin = 'G';
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
}; // namespace Symbols

namespace Colors {
    // Source: https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
    /*
     * This is free and unencumbered software released into the public domain.
     *
     * For more information, please refer to <https://unlicense.org>
     */

    // Regular Colors
    const string Black = "\e[0;30m";
    const string Red = "\e[0;31m";
    const string Green = "\e[0;32m";
    const string Yellow = "\e[0;33m";
    const string Blue = "\e[0;34m";
    const string Purple = "\e[0;35m";
    const string Cyan = "\e[0;36m";
    const string White = "\e[0;37m";

    // Bold
    const string Black = "\e[1;30m";
    const string Red = "\e[1;31m";
    const string Green = "\e[1;32m";
    const string Yellow = "\e[1;33m";
    const string Blue = "\e[1;34m";
    const string Purple = "\e[1;35m";
    const string Cyan = "\e[1;36m";
    const string White = "\e[1;37m";

    // Underline
    const string Black = "\e[4;30m";
    const string Red = "\e[4;31m";
    const string Green = "\e[4;32m";
    const string Yellow = "\e[4;33m";
    const string Blue = "\e[4;34m";
    const string Purple = "\e[4;35m";
    const string Cyan = "\e[4;36m";
    const string White = "\e[4;37m";

    // Background
    const string Black = "\e[40m";
    const string Red = "\e[41m";
    const string Green = "\e[42m";
    const string Yellow = "\e[43m";
    const string Blue = "\e[44m";
    const string Purple = "\e[45m";
    const string Cyan = "\e[46m";
    const string White = "\e[47m";

    // High Intensity
    const string Black = "\e[0;90m";
    const string Red = "\e[0;91m";
    const string Green = "\e[0;92m";
    const string Yellow = "\e[0;93m";
    const string Blue = "\e[0;94m";
    const string Purple = "\e[0;95m";
    const string Cyan = "\e[0;96m";
    const string White = "\e[0;97m";

    // Bold High Intensity
    const string Black = "\e[1;90m";
    const string Red = "\e[1;91m";
    const string Green = "\e[1;92m";
    const string Yellow = "\e[1;93m";
    const string Blue = "\e[1;94m";
    const string Purple = "\e[1;95m";
    const string Cyan = "\e[1;96m";
    const string White = "\e[1;97m";

    // High Intensity backgrounds
    const string Black = "\e[0;100m";
    const string Red = "\e[0;101m";
    const string Green = "\e[0;102m";
    const string Yellow = "\e[0;103m";
    const string Blue = "\e[0;104m";
    const string Purple = "\e[0;105m";
    const string Cyan = "\e[0;106m";
    const string White = "\e[0;107m";

    // Reset
    const string Reset = "\e[0m";
} // namespace Colors

#endif
