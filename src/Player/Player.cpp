#include "Player.h"

#include <iostream>

Player::Player(const std::vector<Vector2>& vertices, Vector2 position, float rotation,
               float health, float armour, float speed, Texture2D* texture)
    : Entity(vertices, position, rotation, health, armour, speed, texture)
{ }

void Player::handleMovement(float deltaTime)
{
    Vector2 moveDirection = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W)) { moveDirection.y -= 1.0f; }
    if (IsKeyDown(KEY_S)) { moveDirection.y += 1.0f; }
    if (IsKeyDown(KEY_A)) { moveDirection.x -= 1.0f; }
    if (IsKeyDown(KEY_D)) { moveDirection.x += 1.0f; }

    if (moveDirection.x != 0.0f && moveDirection.y != 0.0f) {
        moveDirection = Vector2Normalize(moveDirection);
    }

    moveDirection = Vector2Scale(moveDirection, speed * deltaTime);

    move(moveDirection);

    // Vector2 relMousePos = GetWorldToScreen2D(getPosition(), CameraController::Instance->getCamera());
    Vector2 relMousePos = GameManager::Instance->getCameraController().getScreenToWorld(GetMousePosition());
    Vector2 lookDir = Vector2Subtract(relMousePos, getPosition());

    rotation = lookDir.x == 0 ? rotation = 0.0 : atan((lookDir.y)/(lookDir.x)) * RAD2DEG;
    
    if (relMousePos.x < getPosition().x) {
        rotation += 180;
    }
    
    hitBox.setRotation(rotation);

}

void Player::render() {
    if (texture) {
        Vector2 pos = getPosition();
        Rectangle source = { 0, 0, (float)texture->width, (float)texture->height };
        Rectangle dest = { pos.x, pos.y, (float)texture->width, (float)texture->height };
        Vector2 origin = { texture->width/2.0f, texture->height/2.0f };
        DrawTexturePro(*texture, source, dest, origin, getRotation(), WHITE);
    } else {
        getHitBox().renderDebug(RED, BLUE);
    }
}

bool Player::checkInteract(InteractableEntity* &entity) {
    for (auto& turret : GameManager::Instance->getTurrets()) {
        HitBox forwardHitBox = HitBox::rayHitBox( getPosition(), getRotationVector(), INTERACT_DISTANCE);
        if (turret->getHitBox().checkCollision(forwardHitBox)) {
            controllingEntity = turret.get();
            entity = turret.get();
            return true;
        }
    }
    controllingEntity = nullptr;
    entity = nullptr;
    return false;
}

void Player::update(float deltaTime) {
    handleInput(deltaTime);    
}

void Player::handleInput(float deltaTime) {

    // Whenever F is clicked, cast a ray in front.

    if (controllingEntity){
        if (IsKeyPressed(KEY_F)) {
            std::cout << "Detach from entity" << std::endl;
            controllingEntity = nullptr;
        }

        if (IsKeyPressed(KEY_R)) {
            // std::cout << "Reload" << std::endl;
            controllingEntity->interact(TurretCommands::RELOAD);
        }
        if (IsKeyPressed(KEY_E)) {
            // std::cout << "Fire" << std::endl;
            controllingEntity->interact(TurretCommands::FIRE);
        }

        if (IsKeyDown(KEY_A)) {
            // std::cout << "Turn left" << std::endl;
            controllingEntity->interact(TurretCommands::TURNLEFT);
        }
        if (IsKeyDown(KEY_D)) {
            // std::cout << "Turn right" << std::endl;
            controllingEntity->interact(TurretCommands::TURNRIGHT);
        }


    } else {
        handleMovement(deltaTime);
        if (IsKeyPressed(KEY_F)) {
            InteractableEntity* turret = nullptr;
            if (checkInteract(turret)){
                std::cout << "Interacting with turret: " << turret << std::endl;
            } else {
                std::cout << "No interactable entity found" << std::endl;
            }
        }
    }
    
}