#include "Spaceship.h"


Spaceship::Spaceship(std::vector<std::vector<std::unique_ptr<SpaceshipTile>>> tiles): position{Vector2{0,0}}, health{100}, tiles{tiles} {}

void Spaceship::render(){
    for (int i = 0; i < tiles.size(); i++){
        for (int j = 0; j < tiles[i].size(); j++){
            Vector2 pos = { SpaceshipTile::TILE_SIZE*i + position.x, SpaceshipTile::TILE_SIZE*j + position.y };
            tiles[i][j]->render(pos);
        }
    }
}

void Spaceship::update() {

}

