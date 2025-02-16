#include "Bullet.h"



Bullet::Bullet(Vector2 position, float damage, float speed, Vector2 direction, Texture* texture): 
position(position), damage(damage), speed(speed), direction(direction), texture(texture) { id = idCounter++; }

Bullet::Bullet(): position({0, 0}), damage(0), speed(0), direction({0, 0}), texture(nullptr) { id = idCounter++; }



void Bullet::render() {
    float rotation = atan((direction.y - position.y)/(direction.x - position.x)) * RAD2DEG;
    if (rotation < 0) rotation += 180;
    DrawTextureEx(*texture, position, rotation, 1, WHITE);
}


void Bullet::update(float deltaTime) {
    position += Vector2Scale(Vector2Normalize(direction), speed*deltaTime);

    // if bullet is off screen, destroy it

    // else if bullet collides with wall, destroy it
    // Need to get entity somehow
    
    // else if bullet collides with enemy, destroy it
    // Need to get entity somehow

}

