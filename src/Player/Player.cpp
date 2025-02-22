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

    Vector2 mousePos = GetMousePosition();
    rotation = atan((mousePos.y - getPosition().y) / (mousePos.x - getPosition().x)) * RAD2DEG;
    if (mousePos.x < getPosition().x) {
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
        if (turret->getHitBox().checkCollision(hitBox)) {
            entity = turret.get();
            return true;
        }
    }
    entity = nullptr;
    return false;
}

void Player::update(float deltaTime) {
    handleMovement(deltaTime);

    if (IsKeyPressed(KEY_F)) {
        InteractableEntity* turret = nullptr;
        if (checkInteract(turret)){
            std::cout << "Interacting with turret: " << turret << std::endl;
            turret->interact(TurretCommands::FIRE);
        } else {
            std::cout << "No interactable entity found" << std::endl;
        }
    }
}
