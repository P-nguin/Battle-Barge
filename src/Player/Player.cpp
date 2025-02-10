#include "Player.h"

Player::Player(float x, float y, float width, float height, float health, float armour, float speed, Texture2D *texture)
    : Entity(x, y, width, height, health, armour, height, speed, texture)
{ }

void Player::handleMovement(float deltaTime)
{
    float xMove = 0.f;
    float yMove = 0.f;

    if (IsKeyDown(KEY_W)) { yMove -= deltaTime*speed; }
    if (IsKeyDown(KEY_S)) { yMove += deltaTime*speed; }
    if (IsKeyDown(KEY_A)) { xMove -= deltaTime*speed; }
    if (IsKeyDown(KEY_D)) { xMove += deltaTime*speed; }

    move(xMove, yMove);
}

void Player::render() {
    DrawRectangleRec(hitBox, WHITE);
}

void Player::update(float deltaTime) {
    handleMovement(deltaTime);
}