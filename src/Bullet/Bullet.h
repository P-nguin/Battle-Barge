#ifndef BULLET_H
#define BULLET_H

#include "Entity/Entity.h"
#include "raylib.h"
#include <raymath.h>
#include <iostream>

class Bullet : public Entity {
private:
    float damage;
    float speed;
    static int idCounter;
    const int id;

public:
    Bullet(const std::vector<Vector2>& vertices,
           Vector2 position, 
           float rotation,
           float damage, 
           float speed, 
           Texture2D* texture = nullptr);

    float getDamage() const { return damage; }
    float getSpeed() const { return speed; }
    int getId() const { return id; }

    void setDamage(float damage) { this->damage = damage; }
    void setSpeed(float speed) { this->speed = speed; }

    void update(float deltaTime) override;
};

#endif