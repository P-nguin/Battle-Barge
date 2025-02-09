#include "Entity.h"

Entity::Entity(float x, float y, float health, float armour): x{x}, y{y}, health{health}, armour{armour} {}

float Entity::getX() { return x; }
float Entity::getY() { return y; }
float Entity::getHealth() { return health; }
float Entity::getArmour() { return armour; }

void Entity::setX(float x) { this->x = x; }
void Entity::setY(float y) { this->y = y; }
void Entity::setHealth(float health) { this->health = health; }
void Entity::setArmour(float armour) { this->armour = armour; }

void Entity::setPosition(float x, float y) { this->x = x; this->y = y; }
void Entity::move(float x, float y) { this->x += x; this->y += y; }
void Entity::addHealth(float amount) { this->health += health; }
void Entity::addArmour(float amount) { this->armour += armour; }