#ifndef HITBOX_H
#define HITBOX_H

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>

class HitBox {
private:
    struct ProjectionRange {
        float min;
        float max;
    };

    std::vector<Vector2> getAxes() const;
    
    bool hasOverlap(const HitBox& shape1, const HitBox& shape2, Vector2 axis);
    ProjectionRange projectOntoAxis(const HitBox& shape, Vector2 axis);

public:
    std::vector<Vector2> vertices;
    Vector2 position;
    float rotation;

    std::vector<Vector2> getWorldVertices() const; // move back to private later

    HitBox(std::vector<Vector2> vertices, Vector2 position, float rotation);
    
    HitBox() {
        std::cout << "USING DEFUALT CTOR FOR HITBOX" << std::endl;
    } // remove later!!!

    void updatePosition(Vector2 move);
    void updateRotation(float rotate);
    bool checkCollision(const HitBox& other);
    
    void setPosition(Vector2 position) { this->position = position; }
    void setRotation(float rotation) { this->rotation = rotation; }
    Vector2 getPosition() const { return position; }
    float getRotation() const { return rotation; }
    const std::vector<Vector2>& getVertices() const { return vertices; }

    void renderDebug(Color outlineColor, Color fillColor) const;
};

#endif