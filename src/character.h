#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <utility>

class Board;
struct Tile;

class Character {
    protected:
        int maxHealth, health, defense, attack;

    public:
        Character(int maxHealth, int attack, int defense);

        /** Character to be displayed on board */
        virtual char getCharacter() = 0;
        std::string getHealthColor() const;

        /** Attacking power */
        virtual int getAttack() const;
        virtual int getDefense() const;

        /**
         * Update its own health and returns it, could be negative if it dies
         *
         * @returns {new health, damage dealt}
         */
        virtual std::pair<int, int> beAttacked(int attackPower);

        // whether or not this tile can be moved to
        virtual bool canMove(const Tile*) const;

        virtual ~Character();
};

#endif
