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
{ std::cout << "Bullet created: " << id << std::endl; }

void Bullet::update(float deltaTime) {
    float rotationRad = getRotation() * DEG2RAD;
    Vector2 movement = {
        cosf(rotationRad),
        sinf(rotationRad)
    };
    movement = Vector2Scale(Vector2Normalize(movement), speed * deltaTime);
    move(movement);
}