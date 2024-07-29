#include "tile.h"

#include "interactible.h"
#include "constants.h"
#include "player.h"

#include <ostream>

Tile::~Tile() {
    delete enemy;
    delete item;
    delete treasure;
}

char Tile::getCharacter() const {
    if (player != nullptr)
        return player->getCharacter();
    // if (enemy != nullptr) return enemy->getCharacter();
    if (item != nullptr)
        return item->getCharacter();
    // if (treasure != nullptr) return treasure->getCharacter();
    return mapTile;
}

bool Tile::movable() const {
    return player == nullptr && enemy == nullptr && item == nullptr;
}

std::ostream& operator<<(std::ostream& out, Tile& tile) {
    char character = tile.getCharacter();

    out << symbolToColor(character) << character << Color::Reset;

    return out;
}
