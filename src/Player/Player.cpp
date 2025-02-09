#include "Player.h"

Player::handleMovement() {
    float xMove = 0f;
    float yMove = 0f;

    if (IsKeyPressedRepeat('W')) { yMove -= 1; }
    if (IsKeyPressedRepeat('S')) { yMove += 1; }
    if (IsKeyPressedRepeat('A')) { xMove -= 1; }
    if (IsKeyPressedRepeat('D')) { xMove += 1; }

    x += xMove;
    y += yMove;
}