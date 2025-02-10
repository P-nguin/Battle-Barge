#include "Player.h"

Player::Player(float x, float y, float health, float armour, Texture2D *texture) {
}

void Player::handleMovement()
{
    float xMove = 0.f;
    float yMove = 0.f;

    if (IsKeyPressedRepeat('W')) { yMove -= 1; }
    if (IsKeyPressedRepeat('S')) { yMove += 1; }
    if (IsKeyPressedRepeat('A')) { xMove -= 1; }
    if (IsKeyPressedRepeat('D')) { xMove += 1; }

    x += xMove;
    y += yMove;
}