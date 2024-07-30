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

Game* init(std::string& fileName) {
    Player* player;
    Game* game;
    char move;
    std::cout << "Select race: options are (h)uman, (e)lf, (d)warf, (o)rc, and (g)od: "
              << std::flush;
    std::cin >> move;

    switch (move) {
        case 'h': player = new Human(); break;
        case 'e': player = new Elf(); break;
        case 'd': player = new Dwarf(); break;
        case 'o': player = new Orc(); break;
        case 'g': player = new God(); break;
        case 'r': return init(fileName);
        case 'q': return nullptr;
        default: return init(fileName);
    }

    std::cout << "GENERATING NEW MAP WITH " << fileName << " AS FILENAME!\n";

    if (fileName == "") {
        std::stringstream stream{};

        for (int i = 0; i < 5; i++) {
            // stream << defaultFloor << '\n';
            stream << smallFloor << '\n';
        }

        return new Game(player, std::cout, stream, true);
    }

    std::ifstream stream{fileName};

    return new Game(player, std::cout, stream, false);
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

        Game* game = nullptr;
        game = init(fileName);
        bool playerAlive = true;

        if (game == nullptr) {
            std::cout << "Exiting..." << std::endl;

            return 0;
        }

        game->render();

        std::string move;
        std::cout << "Command: " << std::flush;
        while (std::cin >> move && move != "q") {
            if (!game)
                break;
            else if (move == "r") {
                delete game;

                playerAlive = true;
                game = init(fileName);
                if (game == nullptr) {
                    std::cout << "Exiting..." << std::endl;

                    return 0;
                }

                game->render();
            } else if (move == "no" || move == "so" || move == "ea" || move == "we" || move == "ne" || move == "nw" || move == "se" || move == "sw" || move == ArrowKey::StrKeyUp || move == ArrowKey::StrKeyDown || move == ArrowKey::StrKeyRight || move == ArrowKey::StrKeyLeft || move == ArrowKey::StrKeyUpRight || move == ArrowKey::StrKeyRightUp || move == ArrowKey::StrKeyUpLeft || move == ArrowKey::StrKeyLeftUp || move == ArrowKey::StrKeyDownRight || move == ArrowKey::StrKeyRightDown || move == ArrowKey::StrKeyDownLeft || move == ArrowKey::StrKeyLeftDown) {
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
                    game = init(fileName);
                } else
                    game = nullptr;
            } else {
                std::cout << "Command: " << std::flush;
            }
        }

        std::cout << "Exiting..." << std::endl;
        delete game;

        return 0;
    } catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;

        return 1;
    } catch (...) {
        return 1;
    };
}
