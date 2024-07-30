#include "constants.h"
#include "game.h"
#include "layouts.h"
#include "player.h"
#include "races.h"
#include "random.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h> // srand/rand
#include <string>
#include <sys/types.h> // getpid
#include <unistd.h>

using std::cout;
using std::endl;

Player* initPlayer() {
    Player* player = nullptr;
    char move;
    std::cout << Color::BWhite << "Select race: " << Color::Reset << "options are "
              << Color::Yellow << "(h)uman" << Color::Reset << ", " << Color::White << "(e)lf"
              << Color::Reset << ", " << Color::Black << "(d)warf" << ", " << Color::Green
              << "(o)rc" << Color::Reset << ", and " << Color::BIYellow << "(g)od" << Color::Reset
              << ": " << Color::FWhite << std::flush;
    std::cin >> move;
    std::cout << Color::Reset;

    switch (move) {
        case 'h': player = new Human(); break;
        case 'e': player = new Elf(); break;
        case 'd': player = new Dwarf(); break;
        case 'o': player = new Orc(); break;
        case 'g': player = new God(); break;
        case 'r': return initPlayer();
        case 'q': return nullptr;
        default: return initPlayer();
    }

    player->log() << Color::BICyan << " Player character" << Color::Reset << " has spawned.";

    return player;
}

// void playerTurn(Player* player) {
//     char move;
//     std::cin >> move;

// }

int main(int argc, char* argv[]) {
    try {
        int seed = getpid();
        std::string fileName = "";
        if (argc == 3) {
            seed = stoi(std::string(argv[2]));
            fileName = std::string(argv[1]);
        } else if (argc == 2) {
            fileName = std::string(argv[1]);
        }

        randomEngine.seed(seed);

        if (fileName == "default") {
            fileName = "";
        }

        bool isInputMap = fileName == "";

        Game* game = nullptr;

        std::istream* stream = nullptr;
        if (fileName == "") {
            std::stringstream* _stream = new std::stringstream{};
            stream = _stream;

            stream = _stream;

            for (int i = 0; i < 5; i++) {
                *_stream << std::string{oneChamberFloor} << '\n';
            }

            *_stream >> std::noskipws;

            game = new Game(initPlayer(), std::cout, *stream, isInputMap);
        } else {
            std::ifstream* _stream = new std::ifstream{fileName};
            stream = _stream;

            *_stream >> std::noskipws;

            game = new Game(initPlayer(), std::cout, *stream, isInputMap);
        }

        bool playerAlive = true;

        if (game == nullptr) {
            std::cout << "Exiting..." << std::endl;

            return 0;
        }

        game->render();

        std::string move;
        std::cout << "Command: " << Color::FWhite << std::flush;
        while (std::cin >> move && move != "q") {
            std::cout << Color::Reset;
            if (!game)
                break;
            else if (move == "r") {
                delete game;

                playerAlive = true;
                game = new Game(initPlayer(), std::cout, *stream, isInputMap);
                if (game == nullptr) {
                    std::cout << "Exiting..." << std::endl;

                    return 0;
                }

                game->render();
            } else if (move == "no" || move == "so" || move == "ea" || move == "we" ||
                       move == "ne" || move == "nw" || move == "se" || move == "sw" ||
                       move == ArrowKey::StrKeyUp || move == ArrowKey::StrKeyDown ||
                       move == ArrowKey::StrKeyRight || move == ArrowKey::StrKeyLeft ||
                       move == ArrowKey::StrKeyUpRight || move == ArrowKey::StrKeyRightUp ||
                       move == ArrowKey::StrKeyUpLeft || move == ArrowKey::StrKeyLeftUp ||
                       move == ArrowKey::StrKeyDownRight || move == ArrowKey::StrKeyRightDown ||
                       move == ArrowKey::StrKeyDownLeft || move == ArrowKey::StrKeyLeftDown) {
                playerAlive = game->playerMove(stringToDirection(move));
            } else if (move == "u") {
                std::string direction;
                std::cin >> direction;
                playerAlive = game->playerPickup(stringToDirection(direction));
            } else if (move == "a") {
                std::string direction;
                std::cin >> direction;
                playerAlive = game->playerAttack(stringToDirection(direction));
            } else {
                std::cout << "I don't know what '" << move << "' means. Try again\n";

                continue;
            }

            if (!playerAlive) {
                std::cout << "You DEER! Would you like to continue? (Yes/No) " << std::flush;
                delete game;
                std::string choice;
                std::cin >> choice;
                if (choice == "Yes" || choice == "yes" || choice == "y") {
                    playerAlive = true;
                    game = new Game(initPlayer(), std::cout, *stream, isInputMap);
                } else
                    game = nullptr;
            } else {
                std::cout << Color::BWhite << "Command: " << Color::FWhite << std::flush;
            }

            if (game->didWin()) {
                std::cout << Color::BWhite
                          << "Congratulations! You Won! "
                          << "Your score was " << game->getScore()
                          << " Would you like to play again? (Yes/No) "
                          << Color::FWhite << std::flush;
                delete game;
                std::string choice;
                std::cin >> choice;
                if (choice == "Yes" || choice == "yes" || choice == "y") {
                    playerAlive = true;
                    game = new Game(initPlayer(), std::cout, *stream, isInputMap);
                } else
                    game = nullptr;
            }
        }

        std::cout << "Exiting..." << std::endl;
        delete game;
        delete stream;

        return 0;
    } catch (const std::exception& ex) {
        std::cout << Color::IRed << "ERROR: " << Color::Reset << ex.what() << std::endl;

        return 1;
    } catch (...) {
        std::cout << "Uncaught thing" << std::endl;

        return 1;
    };
}
