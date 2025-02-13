#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include <raymath.h>

class Bullet {
    private:
        Vector2 position;
        float damage, speed;
        Vector2 direction;
        Texture *texture;

    public:
        Bullet(Vector2 position, float damage, float speed, Vector2 direction, Texture* texture);
        Bullet();

        Vector2 getPosition() const;
        float getDamage() const;
        float getSpeed() const;
        Vector2 getDirection() const;
        Texture* getTexture() const;

        void setPosition(Vector2 position);
        void setDamage(float damage);
        void setSpeed(float speed);
        void setDirection(Vector2 direction);
        void setTexture(Texture* texture);


        void render();
        void update(float deltaTime);



};

#endif