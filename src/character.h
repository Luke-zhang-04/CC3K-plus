#ifndef CHARACTER_H
#define CHARACTER_H

#include <utility>

class Board;
struct Tile;

class Character {
    protected:
        int maxHealth, health, defense, attack;

    public:
        Character(int maxHealth, int defense, int attack);

        /** Character to be displayed on board */
        virtual char getCharacter() = 0;

        /** Attacking power */
        virtual int getAttack();
        virtual int getDefense();

        /** Update its own health and returns it, could be negative if it dies */
        virtual std::pair<int, int> beAttacked(int attackPower);

        // whether or not this tile can be moved to
        virtual bool canMove(const Tile*) const;

        virtual ~Character();
};

#endif
