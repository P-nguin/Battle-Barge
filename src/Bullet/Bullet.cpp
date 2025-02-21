#include "Bullet.h"

int Bullet::idCounter = 0;

Bullet::Bullet(const std::vector<Vector2>& vertices,
               Vector2 position, 
               float rotation,
               float damage, 
               float speed, 
               Texture2D* texture)
    : Entity(vertices, position, rotation, 1.0f, 0.0f, speed, false, texture),
      damage(damage),
      speed(speed), id{idCounter++}
{}

void Bullet::update(float deltaTime) {
    float rotationRad = getRotation() * DEG2RAD;
    Vector2 movement = {
        cosf(rotationRad) * speed * deltaTime,
        sinf(rotationRad) * speed * deltaTime
    };
    move(movement);
}