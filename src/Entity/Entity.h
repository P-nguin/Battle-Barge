#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include "raylib.h"
#include "HitBox/HitBox.h"

enum class TurretCommands;

class Entity {
    protected:
        HitBox hitBox;
        float health, armour;
        float speed;
        float rotation;
        Texture2D* texture;

    public:
        Entity(const std::vector<Vector2>& vertices, Vector2 position, float rotation,
               float health, float armour, float speed, 
               Texture2D* texture = nullptr);
        
        virtual ~Entity();

        // Position getters/setters
        Vector2 getPosition() const;
        float getRotation() const;
        Vector2 getRotationVector() const;
        float getHealth() const;
        float getArmour() const;
        Texture2D* getTexture() const;
        const HitBox& getHitBox() const;

        virtual void setPosition(Vector2 position);
        virtual void setRotation(float rotation);
        void setHealth(float health);
        void setArmour(float armour);
        void setTexture(Texture2D* texture);

        virtual void move(Vector2 offset);
        virtual void rotate(float angle);
        void addHealth(float amount);
        void addArmour(float amount);

        virtual void render();
        virtual void update(float deltaTime) = 0;
};

#endif