#include <string>

#include "raylib.h"

class Entity {
    protected:
        float x,y, width, height;
        float health, armour;
        float speed;
        bool interactable;

        Texture2D* texture;
        Rectangle hitBox, interactableHitBox;

    public:
        // maybe add another ctor with setting hitbox
        Entity(float x, float y, float width, float height, float health, float armour, float speed, bool interactable=false, Texture2D* texture=nullptr);

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
        virtual void interact(std::string cmd) = 0;
};