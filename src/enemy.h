#ifndef ENEMY_H
#define ENEMY_H
#include "character.h"

class Board;

enum class EnemyType { Vampire, Werewolf, Troll, Goblin, Merchant, Dragon, Phoenix };

enum class CharacterUpdateAction {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
    Attack,
};

class Enemy: public Character {
    private:
        char character;

    protected:
        CharacterUpdateAction walk(int x, int y);

    public:
        Enemy(EnemyType, Board&);

        char getCharacter();

        virtual CharacterUpdateAction update(int x, int y);
};

#endif
