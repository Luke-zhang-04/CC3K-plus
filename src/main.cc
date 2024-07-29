#include "game.h"
#include "player.h"
#include "races.h"
#include "random.h"

#include <iostream>
#include <stdlib.h> // srand/rand
#include <string>
#include <sstream>
#include <sys/types.h> // getpid
#include <unistd.h>
#include <fstream>

#include "default_floor.h"

using std::cout;
using std::endl;

Game* init(std::string& fileName) {
    Player* player;
    Game* game;
    char move;
    std::cin >> move;

    switch (move) {
        case 'h': player = new Human(); break;
        case 'e': player = new Elf(); break;
        case 'd': player = new Dwarf(); break;
        case 'o': player = new Orc(); break;
        case 'r': return init(fileName);
        case 'q': return nullptr;
        default: return init(fileName);
    }


    if (fileName == "") {
        std::stringstream stream{};

        for (int i = 0; i < 5; i++) {
            stream << defaultFloor << '\n';
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

        std::cout << "PENIS" << std::endl;

        game->render();

        std::string move;
        while (std::cin >> move && move != "q") {
            if (!game)
                break;
            else if (move == "r") {
                delete game;
                game = init(fileName);
            } else if (move == "no" || move == "so" || move == "ea" || move == "we" || move == "ne" ||
                    move == "nw" || move == "se" || move == "sw") {
                playerAlive = game->playerMove(stringToDirection(move));
            } else if (move == "u") {
                std::string direction;
                std::cin >> direction;
                playerAlive = game->playerPickup(stringToDirection(move));
            } else if (move == "a") {
                std::string direction;
                std::cin >> direction;
                playerAlive = game->playerAttack(stringToDirection(move));
            } else {
                continue;
            }
        }
        delete game;

        return 0;
    } catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;

        return 1;
    } catch (...) {
        return 1;
    };
}
