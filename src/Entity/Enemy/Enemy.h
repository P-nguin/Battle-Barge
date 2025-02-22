#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <iostream>
#include "raylib.h"
#include <map>

#include "../../GameManager/GameManager.h"
#include "../../Bullet/Bullet.h"
#include "../Entity.h"

class Bullet;
class GameManager;

class Enemy : public Entity {

    private:
        float atkTimer = 0.0f;
        Entity *target = nullptr;
        std::map<int, float> recentBulletHitCooldowns;

    protected:
        float damage;
        float attackSpeed;
        Vector2 targetPosition;

    public:
        Enemy(const std::vector<Vector2>& vertices, Vector2 position, float rotation,
               float health, float armour, float speed, Texture2D* texture, float damage, float atkSpeed, Vector2 targetPosition);

        virtual ~Enemy() = default;

        float getDamage() const { return damage; }

        bool takeDamage(Bullet *bullet);

        void update(float deltaTime) override;
        void render() override;
};

#endif