#include "raylib.h"

class Entity {
    protected:
        float x,y;
        float health, armour;

        Texture2D texture;

        Rectangle hitBox;

    public:
        Entity(float x, float y, float health, float armour);

        float getX();
        float getY();
        float getHealth();
        float getArmour();

        void setX(float x);
        void setY(float y);
        void setHealth(float health);
        void setArmour(float armour);

        void setPosition(float x, float y);
        void move(float x, float y);
        void addHealth(float amount);
        void addArmour(float amount);

        virtual void render();
        virtual void update(float deltaTime);
};