#ifndef PLAYER_H
#define PLAYER_H

#include "Entity/Entity.h"
#include "raylib.h"
#include <iostream>
#include "GameManager/GameManager.h"
#include "Turret/Turret.h"
#include "CameraController/CameraController.h"
#include "HitBox/HitBox.h"

class Player : public Entity {
    private:
        const float INTERACT_DISTANCE = 30.0f;
        InteractableEntity* controllingEntity = nullptr;
        bool checkInteract(InteractableEntity* &entity);
    public:
        Player(const std::vector<Vector2>& vertices, Vector2 position, float rotation, float health, float armour, float speed, Texture2D* texture = nullptr);
        void handleInput(float deltaTime);
        void handleMovement(float deltaTime);
        void render() override;
        void update(float deltaTime) override;
};

#endif