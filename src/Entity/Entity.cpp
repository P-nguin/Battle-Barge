#include "Entity.h"

Entity::Entity(float x, float y, float width, float height, float health, float armour, float speed, bool interactable, Texture2D *texture):
    x{x}, y{y}, health{health}, armour{armour}, height{height}, width{width}, speed{speed}, interactable{interactable}, texture{texture}
{
    hitBox.x = x; hitBox.y = y; hitBox.width = width; hitBox.height = height;
    // probably want interactableHitBox to be slightly bigger
    if(interactable) {
        interactableHitBox.x = x; interactableHitBox.y = y; interactableHitBox.width = width; interactableHitBox.height = height;
    }
}

Entity::~Entity()
{
    if(texture) {
        UnloadTexture(*texture);
        delete texture;
    }
}

void Entity::render() {
    // Change later when we have a texture
    DrawRectangleRec(hitBox, RED);
}

float Entity::getX() const { return x; }
float Entity::getY() const { return y; }
float Entity::getHealth() const { return health; }
float Entity::getArmour() const { return armour; }
Texture2D* Entity::getTexture() const { return texture; }

void Entity::setX(float x) { 
    this->x = x;
    hitBox.x = x;
}
void Entity::setY(float y) { 
    this->y = y; 
    hitBox.y = y;
}
void Entity::setHealth(float health) { this->health = health; }
void Entity::setArmour(float armour) { this->armour = armour; }
void Entity::setTexture(Texture2D* texture) { this->texture = texture; }

void Entity::setPosition(float x, float y) { 
    this->x = x; this->y = y; 
    hitBox.x = x; hitBox.y = y;
}
void Entity::move(float x, float y) { 
    this->x += x; this->y += y; 
    hitBox.x += x; hitBox.y += y;
    // maybe x and y are redundent with the hitbox? idk
}
void Entity::addHealth(float amount) { this->health += health; }
void Entity::addArmour(float amount) { this->armour += armour; }