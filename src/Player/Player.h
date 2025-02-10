#include "Entity/Entity.h"
#include "raylib.h"

class Player : Entity {
public:
    Player(float x, float y, float width, float height, float health, float armour, float speed, Texture2D* texture);

    void handleMovement(float deltaTime);
    void render() override;
    void update(float deltaTime) override;
    void interact(std::string cmd) override {} // Cannot interact with player for now I guess
};