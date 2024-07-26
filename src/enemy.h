#ifndef ENEMY_H
#define ENEMY_H
#include <string.h>

#include "board.h"
#include "character.h"

enum class EnemyType {
    Vampire,
    Werewolf,
    Troll,
    Goblin,
    Merchant,
    Dragon,
    Phoenix
};

class Enemy: public Character {
    private:
        char character;

    public:
        Enemy(EnemyType, Board&);

        char getCharacter();

        virtual std::string update(int x, int y);
};

class Dragon: public Enemy {};

class Merchant: public Enemy {};

#endif
