#ifndef ENEMY_H
#define ENEMY_H

#include "../Entity.h"

class Enemy : public Entity {

    protected:
        float damage;
        float attackSpeed;

    public:

        Enemy();
        Enemy(const std::vector<Vector2>& vertices, Vector2 position, float rotation,
               float health, float armour, float speed, float damage, Texture2D* texture);

        virtual ~Enemy() = default;


};

#endif