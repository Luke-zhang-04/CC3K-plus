#include "game.h"

#include <ostream>

Game::~Game() {
    delete currentBoard;
    delete player;
}

// TODO this is not trivial
void Game::nextLevel() {
    delete currentBoard;
    // currentBoard = new Board{};
}

void Game::render() {
    currentBoard->render(output);
}
