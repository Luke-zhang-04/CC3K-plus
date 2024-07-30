#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>
#include <string>
#include <utility>

using std::string;

constexpr const uint8_t levelCount = 5;

namespace Symbol {
    constexpr const char BarrierSuit = 'B';
    constexpr const char Compass = 'C';

    constexpr const char Vampire = 'V';
    constexpr const char Werewolf = 'W';
    constexpr const char Troll = 'T';
    constexpr const char Goblin = 'N';
    constexpr const char Merchant = 'M';
    constexpr const char Dragon = 'D';
    constexpr const char Phoenix = 'X';

    constexpr const char Player = '@';

    constexpr const char Potion = 'P';

    constexpr const char Treasure = 'G';

    constexpr const char Stairs = '\\';
    constexpr const char WallVert = '|';
    constexpr const char WallHorz = '-';
    constexpr const char Door = '+';
    constexpr const char Passage = '#';
    constexpr const char FloorTile = '.';
    constexpr const char Blank = ' ';
}; // namespace Symbol

namespace SpawnRates {
    const uint8_t EnemyTotal = 0;
    const uint8_t EnemyTotalRate = 18;
    const uint8_t EnemyVampireRate = 3;
    const uint8_t EnemyWerewolfRate = 4;
    const uint8_t EnemyTrollRate = 2;
    const uint8_t EnemyGoblinRate = 5;
    const uint8_t EnemyMerchantRate = 2;
    const uint8_t EnemyPhoenixRate = 2;

    const uint8_t PotionTotal = 10;
    const uint8_t PotionTotalRate = 6;
    const uint8_t PotionRate = 1;

    const uint8_t GoldTotal = 10;
    const uint8_t GoldTotalRate = 8;
    const uint8_t GoldNormalRate = 5;
    const uint8_t GoldSmallHoardRate = 2;
    const uint8_t GoldDragonHoardRate = 1;

    const uint8_t Total = SpawnRates::EnemyTotal + SpawnRates::PotionTotal + SpawnRates::GoldTotal;

    static_assert(
        Total == SpawnRates::EnemyTotal + SpawnRates::PotionTotal + SpawnRates::GoldTotal,
        "Spawn totals don't add up (how tf did we get here)"
    );
    static_assert(
        EnemyTotalRate == EnemyVampireRate + EnemyWerewolfRate + EnemyTrollRate + EnemyGoblinRate +
                              EnemyMerchantRate + EnemyPhoenixRate,
        "Enemy rate totals don't add up"
    );
    static_assert(
        GoldTotalRate == GoldNormalRate + GoldSmallHoardRate + GoldDragonHoardRate,
        "Gold rate totals don't add up"
    );
} // namespace SpawnRates

enum class CardinalDirection {
    NorthWest,
    North,
    NorthEast,
    West,
    Center,
    East,
    SouthWest,
    South,
    SouthEast,
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

namespace TreasureType {
    constexpr const uint8_t SmallGoldPile = 1;
    constexpr const uint8_t SmallHoard = 2;
    constexpr const uint8_t MerchantHoard = 4;
    constexpr const uint8_t DragonHoard = 6;
} // namespace TreasureType

namespace ArrowKey {
    constexpr const char KeyUp = 72;
    constexpr const char KeyDown = 80;
    constexpr const char KeyLeft = 75;
    constexpr const char KeyRight = 77;

    const std::string StrKeyUp = std::string(1, 27) + (char) 91 + 'A';
    const std::string StrKeyDown = std::string(1, 27) + (char) 91 + 'B';
    const std::string StrKeyRight = std::string(1, 27) + (char) 91 + 'C';
    const std::string StrKeyLeft = std::string(1, 27) + (char) 91 + 'D';
    const std::string StrKeyUpRight = ArrowKey::StrKeyUp + ArrowKey::StrKeyRight;
    const std::string StrKeyRightUp = ArrowKey::StrKeyRight + ArrowKey::StrKeyUp;
    const std::string StrKeyUpLeft = ArrowKey::StrKeyUp + ArrowKey::StrKeyLeft;
    const std::string StrKeyLeftUp = ArrowKey::StrKeyLeft + ArrowKey::StrKeyUp;
    const std::string StrKeyDownRight = ArrowKey::StrKeyDown + ArrowKey::StrKeyRight;
    const std::string StrKeyRightDown = ArrowKey::StrKeyRight + ArrowKey::StrKeyDown;
    const std::string StrKeyDownLeft = ArrowKey::StrKeyDown + ArrowKey::StrKeyLeft;
    const std::string StrKeyLeftDown = ArrowKey::StrKeyLeft + ArrowKey::StrKeyDown;
}; // namespace ArrowKey

namespace Color {
    // https://en.wikipedia.org/wiki/ANSI_escape_code

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

    // Faint
    const string FBlack = "\e[2;30m";
    const string FRed = "\e[2;31m";
    const string FGreen = "\e[2;32m";
    const string FYellow = "\e[2;33m";
    const string FBlue = "\e[2;34m";
    const string FPurple = "\e[2;35m";
    const string FCyan = "\e[2;36m";
    const string FWhite = "\e[2;37m";

    // Italic
    const string ItBlack = "\e[3;30m";
    const string ItRed = "\e[3;31m";
    const string ItGreen = "\e[3;32m";
    const string ItYellow = "\e[3;33m";
    const string ItBlue = "\e[3;34m";
    const string ItPurple = "\e[3;35m";
    const string ItCyan = "\e[3;36m";
    const string ItWhite = "\e[3;37m";

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

    // Italic High Intensity
    const string ItIBlack = "\e[3;90m";
    const string ItIRed = "\e[3;91m";
    const string ItIGreen = "\e[3;92m";
    const string ItIYellow = "\e[3;93m";
    const string ItIBlue = "\e[3;94m";
    const string ItIPurple = "\e[3;95m";
    const string ItICyan = "\e[3;96m";
    const string ItIWhite = "\e[3;97m";

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

bool isEnemy(char symbol);

std::string symbolToColor(char symbol);

CardinalDirection stringToDirection(std::string str);

std::string directionToString(CardinalDirection);

std::pair<int, int> directionToDisplacement(CardinalDirection);

#endif
