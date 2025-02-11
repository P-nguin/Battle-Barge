#ifndef ENTITY_H
#define ENTITY_H
#include "raylib.h"

class Entity {
    protected:
        float x,y, width, height;
        float health, armour;
        bool interactable;

        Texture2D* texture;
        Rectangle hitBox, interactableHitBox;

    public:
        // maybe add another ctor with setting hitbox
        Entity(float x, float y, float width=32, float height=32, float health=10, float armour=0, bool interactable=false, Texture2D* texture=nullptr);
        Entity() {}
        ~Entity();

        float getX() const;
        float getY() const;
        float getHealth() const;
        float getArmour() const;
        Texture2D* getTexture() const;

        void setX(float x);
        void setY(float y);
        void setHealth(float health);
        void setArmour(float armour);
        void setTexture(Texture2D* texture);
        void setPosition(float x, float y);

        void move(float x, float y);
        void addHealth(float amount);
        void addArmour(float amount);

        virtual void render();
        virtual void update(float deltaTime) = 0;
};

#endif
