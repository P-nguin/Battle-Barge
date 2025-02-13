#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include "raylib.h"
#include "HitBox/HitBox.h"

class Entity {
    protected:
        HitBox hitBox;
        HitBox interactableHitBox;  // Optional hitbox for interaction
        float health, armour;
        float speed;
        bool interactable;
        Texture2D* texture;

    public:
        Entity(const std::vector<Vector2>& vertices, Vector2 position, float rotation,
               float health, float armour, float speed, bool interactable = false, 
               Texture2D* texture = nullptr);
        
        ~Entity();

        // Position getters/setters
        Vector2 getPosition() const;
        float getRotation() const;
        float getHealth() const;
        float getArmour() const;
        Texture2D* getTexture() const;
        const HitBox& getHitBox() const;

        void setPosition(Vector2 position);
        void setRotation(float rotation);
        void setHealth(float health);
        void setArmour(float armour);
        void setTexture(Texture2D* texture);

        void move(Vector2 offset);
        void rotate(float angle);
        void addHealth(float amount);
        void addArmour(float amount);

        virtual void render();
        virtual void update(float deltaTime) = 0;
        // virtual void interact(const std::string& cmd) = 0;
};

#endif