#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include <raymath.h>

class Bullet {
    private:
        int id;
        Vector2 position;
        float damage, speed;
        Vector2 direction;
        Texture *texture;

        static int idCounter;

    public:
        Bullet(Vector2 position, float damage, float speed, Vector2 direction, Texture* texture);
        Bullet();

        Vector2 getPosition() const { return position; };
        float getDamage() const { return damage; };
        float getSpeed() const { return speed; };
        Vector2 getDirection() const { return direction; };
        Texture* getTexture() const { return texture; };

        void setPosition(Vector2 position){ this->position = position; }
        void setDamage(float damage){ this->damage = damage; }
        void setSpeed(float speed){ this->speed = speed; }
        void setDirection(Vector2 direction){ this->direction = direction; }
        void setTexture(Texture* texture){ this->texture = texture; }

        void render();
        void update(float deltaTime);



};

#endif