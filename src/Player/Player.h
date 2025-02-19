#ifndef PLAYER_H
#define PLAYER_H

#include "Entity/Entity.h"
#include "raylib.h"
#include <iostream>

class Player : public Entity {
    public:
        Player(const std::vector<Vector2>& vertices, Vector2 position, float rotation, float health, float armour, float speed, Texture2D* texture = nullptr);
        void handleMovement(float deltaTime);
        void render() override;
        void update(float deltaTime) override;
    };

#endif