#include "Turret.h"

Turret::Turret(const std::vector<Vector2>& vertices, Vector2 position,
               float forwardAngle, float health, float armour,
               float turnRate, float range, float fireRate,
               int ammo, float reloadTime, Texture2D* texture,
               AmmoType ammoType, TurretType turretType)
    : Entity(vertices, position, forwardAngle, health, armour, 0, true, texture),
      forwardAngle(forwardAngle),
      localAngle(0.0f),
      globalAngle(forwardAngle),
      MAXIMUN_ANGLE(45.0f),  // You might want to make this configurable
      turnRate(turnRate),
      range(range),
      fireRate(fireRate),
      accuracy(0.0f),
      MAXIMUN_ACCURACY(0.0f),
      MINIMUN_ACCURACY(0.0f),
      MAXIMUN_AMMO(ammo),
      ammo(ammo),
      reloadTime(reloadTime),
      ammoType(ammoType),
      turretType(turretType)
{
    std::vector<Vector2> interactVertices;
    float padding = 5.0f;
    for (const auto& vertex : vertices) {
        interactVertices.push_back({ vertex.x + padding, vertex.y + padding });
    }
    interactableHitBox = HitBox(interactVertices, position, forwardAngle);
}

void Turret::fire() {
    if (ammo > 0) {
        std::cout << "FIRE" << std::endl;
        ammo--;
    } else {
        std::cout << "NO AMMO" << std::endl;
    }
}

void Turret::rotate(float angle) {
    localAngle += angle;
    localAngle = fmod(localAngle, 360.0f);

    localAngle = std::max(localAngle, -MAXIMUN_ANGLE);
    localAngle = std::min(localAngle, MAXIMUN_ANGLE);

    globalAngle = forwardAngle + localAngle;
    globalAngle = fmod(globalAngle, 360.0f);
    
    setRotation(globalAngle);
}

void Turret::render() {
    if (texture) {
        // Draw texture rotated at turret position
        Vector2 pos = getPosition();
        Rectangle source = { 0, 0, (float)texture->width, (float)texture->height };
        Rectangle dest = { pos.x, pos.y, (float)texture->width, (float)texture->height };
        Vector2 origin = { texture->width/2.0f, texture->height/2.0f };
        DrawTexturePro(*texture, source, dest, origin, globalAngle, WHITE);
    } else {
        // Debug rendering
        const std::vector<Vector2>& vertices = getHitBox().getWorldVertices();
        
        // Draw filled polygon
        if (vertices.size() >= 3) {
            Vector2* vertexArray = new Vector2[vertices.size()];
            for (size_t i = 0; i < vertices.size(); i++) {
                vertexArray[i] = vertices[i];
            }
            
            DrawTriangleFan(vertexArray, vertices.size(), BROWN);
            delete[] vertexArray;
        }
        
        // Draw hitbox outline
        for (size_t i = 0; i < vertices.size(); i++) {
            Vector2 start = vertices[i];
            Vector2 end = vertices[(i + 1) % vertices.size()];
            DrawLineV(start, end, BLACK);
        }

        // Draw interaction hitbox if interactable
        if (interactable) {
            const std::vector<Vector2>& interactVertices = interactableHitBox.getWorldVertices();
            for (size_t i = 0; i < interactVertices.size(); i++) {
                Vector2 start = interactVertices[i];
                Vector2 end = interactVertices[(i + 1) % interactVertices.size()];
                DrawLineV(start, end, PURPLE);
            }
        }

        // Draw direction indicator
        Vector2 pos = getPosition();
        Vector2 dir = { cosf(globalAngle * DEG2RAD), sinf(globalAngle * DEG2RAD) };
        Vector2 end = Vector2Add(pos, Vector2Scale(dir, 20.0f));
        DrawLineV(pos, end, RED);
    }
}

void Turret::interact(TurretCommands cmd) {
    switch (cmd) {
        case TurretCommands::FIRE:
            fire();
            break;
        case TurretCommands::TURNLEFT:
            rotate(-turnRate);
            break;
        case TurretCommands::TURNRIGHT:
            rotate(turnRate);
            break;
        case TurretCommands::RELOAD:
            reload();
            break;
    }
}

void Turret::reload() {
    // insta reload for now
    ammo = MAXIMUN_AMMO;
}