#include "SpaceshipTile.h"

SpaceshipTile::SpaceshipTile(float health, float maxHealth, float defense, float maxDefense): 
health{health}, maxHealth{maxHealth}, 
defense{defense}, maxDefense{maxDefense}
{}

WallTile::WallTile(): SpaceshipTile(100, 100, 0,0) {}
WallTile::WallTile(float health, float maxHealth, float defense, float maxDefense): SpaceshipTile(health,maxHealth,defense,maxDefense) {}


FloorTile::FloorTile(): SpaceshipTile(100, 100, 0,0) {}
FloorTile::FloorTile(float health, float maxHealth, float defense, float maxDefense): SpaceshipTile(health,maxHealth,defense,maxDefense) {}

TurretTile::TurretTile(): SpaceshipTile(100, 100, 0,0), turret(nullptr) {}
TurretTile::TurretTile(Turret *turret): SpaceshipTile(turret->getHealth() ,turret->getHealth(),turret->getArmour(),turret->getArmour()), turret{turret} {}


void SpaceshipTile::render(Vector2 pos) {
    Texture2D texture = LoadTexture("Spaceship/UnkownTile.png");
    DrawTexture(texture, pos.x, pos.y, WHITE );
}
void WallTile::render(Vector2 pos) {
    Texture2D texture = LoadTexture("Spaceship/WallTile.png");
    DrawTexture(texture, pos.x, pos.y, WHITE );
}
void FloorTile::render(Vector2 pos) {
    Texture2D texture = LoadTexture("Spaceship/FloorTile.png");
    DrawTexture(texture, pos.x, pos.y, WHITE );
}

void TurretTile::render(Vector2 pos) {
    turret->setPosition(pos);
    turret->render();
}
