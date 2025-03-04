#include "Spaceship.h"


Spaceship::Spaceship(std::vector<std::vector<SpaceshipTile*>> tiles): position{Vector2{0,0}}, health{100}, tiles{tiles} {}

void Spaceship::render(){
    for (int i = 0; i < tiles.size(); i++){
        for (int j = 0; j < tiles[i].size(); j++){
            tiles[i][j]->render(this);
        }
    }
}

void Spaceship::update() {

}

