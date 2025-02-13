#include "Entity.h"

Entity::Entity(const std::vector<Vector2>& vertices, Vector2 position, float rotation, float health, float armour, float speed, bool interactable, Texture2D* texture)
    : hitBox(vertices, position, rotation), interactableHitBox(),
      health(health), armour(armour), speed(speed), rotation(0),
      interactable(interactable), texture(texture)
{
    if (interactable) {
        std::vector<Vector2> interactVertices;
        float padding = 5.0f;
        for (const auto& vertex : vertices) {
            interactVertices.push_back({ vertex.x + padding, vertex.y + padding });
        }
        interactableHitBox = HitBox(interactVertices, position, rotation);
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
    const std::vector<Vector2>& vertices = hitBox.getWorldVertices();
    for (size_t i = 0; i < vertices.size(); i++) {
        Vector2 start = vertices[i];
        Vector2 end = vertices[(i + 1) % vertices.size()];
        DrawLineV(start, end, RED);
    }
}

Vector2 Entity::getPosition() const { return hitBox.getPosition(); }
float Entity::getRotation() const { return hitBox.getRotation(); }
float Entity::getHealth() const { return health; }
float Entity::getArmour() const { return armour; }
Texture2D* Entity::getTexture() const { return texture; }
const HitBox& Entity::getHitBox() const { return hitBox; }

void Entity::setPosition(Vector2 position) { 
    hitBox.setPosition(position);
    if (interactable) {
        interactableHitBox.setPosition(position);
    }
}

void Entity::setRotation(float rotation) { 
    hitBox.setRotation(rotation);
    if (interactable) {
        interactableHitBox.setRotation(rotation);
    }
}

void Entity::setHealth(float health) { this->health = health; }
void Entity::setArmour(float armour) { this->armour = armour; }
void Entity::setTexture(Texture2D* texture) { this->texture = texture; }

void Entity::move(Vector2 offset) { 
    hitBox.updatePosition(offset);
    if(interactable) {
        interactableHitBox.updatePosition(offset);
    }
}

void Entity::rotate(float angle) {
    hitBox.updateRotation(angle);  // Update rotation with no position change
    if (interactable) {
        interactableHitBox.updateRotation(angle);
    }
}

void Entity::addHealth(float amount) { health += amount; }
void Entity::addArmour(float amount) { armour += amount; }
