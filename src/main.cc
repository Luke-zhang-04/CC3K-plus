#include <iostream>
#include <stdlib.h> // srand/rand
#include <string>
#include <sys/types.h> // getpid
#include <unistd.h>
#include "player.h"
#include "races.h"
#include "game.h"
using namespace std;

Game* init() {
    Player* player;
    Game* game;
    char move;
    std::cin >> move;
    switch (move) {
        case 'h':
            player = new Human();
        case 'e':
            player = new Elf();
        case 'd':
            player = new Dwarf();
        case 'o':
            player = new Orc();
        case 'r':
            return init();
        case 'q':
            return nullptr;
        default:
            return init();
    }
    return new Game(player, std::cout, std::cin); //WIP
}

// void playerTurn(Player* player) {
//     char move;
//     std::cin >> move;

// }

int main(int argc, char* argv[]) {
    int seed = getpid();
    if (argc == 2)
        seed = stoi(string{argv[1]});
    srand(seed); // only seed the random number generator once!

    Game* game;
    game = init();
    bool playerAlive = true;

    std::string move;
    while (std::cin >> move && move != "q") {
        if (!game) break;
        else if (move == "r") {
                delete game;
                game = init();
        }
        else if (move == "no" || move == "so" || move == "ea" || move == "we" || move == "ne" || move == "nw" || move == "se" || move == "sw") {
            playerAlive = game->playerMove(stringToDirection(move));
        }
        else if (move == "u") {
            std::string direction;
            std::cin >> direction;
            playerAlive = game->playerPickup(stringToDirection(move));
        }
        else if (move == "a") {
            std::string direction;
            std::cin >> direction;
            playerAlive = game->playerAttack(stringToDirection(move));
        }
        else {
            continue;
        }
    }
    delete game;
}
