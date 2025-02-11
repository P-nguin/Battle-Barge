#ifndef PLAYER_H
#define PLAYER_H
#include "Entity/Entity.h"
#include "raylib.h"

class Player : Entity {

    public:
        Player(float x, float y, float health, float armour, Texture2D* texture);

        void handleMovement();
        void render() override = 0;
        void update(float deltaTime) override = 0;
};

#endif

