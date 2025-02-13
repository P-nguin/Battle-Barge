#include "Bullet.h"



Bullet::Bullet(Vector2 position, float damage, float speed, Vector2 direction, Texture* texture): 
position(position), damage(damage), speed(speed), direction(direction), texture(texture) {}

Bullet::Bullet(): position({0, 0}), damage(0), speed(0), direction({0, 0}), texture(nullptr) {}

Vector2 Bullet::getPosition() const { return position; }
float Bullet::getDamage() const { return damage; }
float Bullet::getSpeed() const { return speed; }
Vector2 Bullet::getDirection() const { return direction; }
Texture* Bullet::getTexture() const { return texture; }

void Bullet::setPosition(Vector2 position) { this->position = position; }
void Bullet::setDamage(float damage) { this->damage = damage; }
void Bullet::setSpeed(float speed) { this->speed = speed; }
void Bullet::setDirection(Vector2 direction) { this->direction = direction; }
void Bullet::setTexture(Texture* texture) { this->texture = texture; }


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

