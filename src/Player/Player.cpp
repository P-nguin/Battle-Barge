#include "Player.h"

#include <iostream>

Player::Player(const std::vector<Vector2>& vertices, Vector2 position, float rotation,
               float health, float armour, float speed, Texture2D* texture)
    : Entity(vertices, position, rotation, health, armour, speed, false, texture)
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

    moveDirection.x *= speed * deltaTime;
    moveDirection.y *= speed * deltaTime;

    move(moveDirection);

    Vector2 mousePos = GetMousePosition();
    rotation = atan((mousePos.y - getPosition().y) / (mousePos.x - getPosition().x)) * RAD2DEG;
    if (mousePos.x < getPosition().x) {
        rotation += 180;
    }

}

void Player::render() {
    if (texture) {
        DrawTextureEx(*texture, getPosition(), rotation, 1, WHITE);
    } else {
        getHitBox().renderDebug(RED, BLUE);
    }
}

void Player::update(float deltaTime) {
    handleMovement(deltaTime);
}
