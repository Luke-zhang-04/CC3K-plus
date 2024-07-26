#include "enemy.h"

Enemy::Enemy(EnemyType enemyType, Board& board): Character{board, 0, 0, 0}, character{0} {
    switch (enemyType) {
        case EnemyType::Vampire:
            maxHealth = 50;
            attack = 25;
            defense = 25;
            character = 'V';
            break;
        case EnemyType::Werewolf:
            maxHealth = 120;
            attack = 30;
            defense = 5;
            character = 'W';
            break;
        case EnemyType::Troll:
            maxHealth = 120;
            attack = 25;
            defense = 15;
            character = 'T';
            break;
        case EnemyType::Goblin:
            maxHealth = 70;
            attack = 5;
            defense = 10;
            character = 'G';
            break;
        case EnemyType::Merchant:
            maxHealth = 30;
            attack = 70;
            defense = 5;
            character = 'M';
            break;
        case EnemyType::Dragon:
            maxHealth = 150;
            attack = 20;
            defense = 20;
            character = 'D';
            break;
        case EnemyType::Phoenix:
            maxHealth = 50;
            attack = 35;
            defense = 20;
            character = 'X';
            break;
    }

    health = maxHealth;
}

char Enemy::getCharacter() {
    return character;
}
