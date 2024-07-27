#ifndef DRAGON_PROTECTED_H
#define DRAGON_PROTECTED_H

class Dragon;

class DragonProtected {
    protected:
        // if dragon is not nullptr, it is currently protected
        Dragon* dragon;

    public:
        void dragonDead();
        DragonProtected(Dragon* d): dragon{d} {};
};

#endif // DRAGON_PROTECTED_H
