#include "Entity/Entity.h"
#include "raylib.h"

class Player : Entity {

    public:
        Player(float x, float y, float health, float armour, Texture2D* texture);

        void handleMovement();
        void render() override;
        void update(float deltaTime) override;
};