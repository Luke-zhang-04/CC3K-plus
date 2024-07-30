#ifndef RETRIEVABLE_H
#define RETRIEVABLE_H

class Player;
class Enemy;

class Retrievable {
    private:
        // enemy protecting this retrievable, not possible to collect while exists
        Enemy* protectedBy = nullptr;

    protected:
        // what to do if it is unprotected and a retrieve attempt occurs
        virtual bool retrieved(Player* player) = 0;

    public:
        // allow you to construct with a protector
        Retrievable(Enemy* protectedBy);
        Retrievable();
        virtual char getCharacter() = 0;

        // attempt to retrieve this object, if not protected
        bool onRetrieve(Player* player);

        // getter for whether this item is protected or not
        bool isProtected();

        // these two make it write only
        // for when whatever is protecting this dies
        void protectorDies();

        // add a protector if one does not already exist
        void setProtector(Enemy*);

        virtual ~Retrievable() {}
};

#endif
