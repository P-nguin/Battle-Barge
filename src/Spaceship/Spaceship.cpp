#include "Spaceship.h"


Spaceship::Spaceship(): position{Vector2{0,0}} {
    tiles = {   {SpaceShipTile::WALL, SpaceShipTile::WALL, SpaceShipTile::WALL, SpaceShipTile::WALL},
                {SpaceShipTile::WALL, SpaceShipTile::FLOOR, SpaceShipTile::FLOOR, SpaceShipTile::WALL},
                {SpaceShipTile::WALL, SpaceShipTile::FLOOR, SpaceShipTile::FLOOR, SpaceShipTile::TURRET},
                {SpaceShipTile::WALL, SpaceShipTile::WALL, SpaceShipTile::WALL, SpaceShipTile::WALL},
 };
}

void Spaceship::render(){

}

void Spaceship::update() {

}
