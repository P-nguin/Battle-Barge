#include "Entity/Entity.h"
#include "raylib.h"

class Player : Entity {

    public:
        void handleMovement();
        void render();
        void update(float deltaTime);

};