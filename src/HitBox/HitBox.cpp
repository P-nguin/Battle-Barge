#include "HitBox.h"

HitBox::HitBox(std::vector<Vector2> vertices, Vector2 position, float rotation) {
        Vector2 center = {0, 0};
        for (const auto& vertex : vertices) {
            center.x += vertex.x;
            center.y += vertex.y;
        }
        center.x /= vertices.size();
        center.y /= vertices.size();

        this->vertices.reserve(vertices.size());
        for (const auto& vertex : vertices) {
            this->vertices.push_back({
                vertex.x - center.x,
                vertex.y - center.y
            });
        }

        this->position = position;
        this->rotation = rotation;
    }

bool HitBox::checkCollision(const HitBox& other) {
    std::vector<Vector2> axes1 = getAxes();
    std::vector<Vector2> axes2 = other.getAxes();
    
    for (const Vector2& axis : axes1) {
        if (!hasOverlap(*this, other, axis)) return false;
    }
    
    for (const Vector2& axis : axes2) {
        if (!hasOverlap(*this, other, axis)) return false;
    }
    
    return true;
}

std::vector<Vector2> HitBox::getAxes() const {
    std::vector<Vector2> axes;
    std::vector<Vector2> worldVertices = getWorldVertices();
    
    for (size_t i = 0; i < worldVertices.size(); i++) {
        Vector2 p1 = worldVertices[i];
        Vector2 p2 = worldVertices[(i + 1) % worldVertices.size()];
        
        // Get edge vector
        Vector2 edge = Vector2Subtract(p2, p1);
        
        // Get perpendicular vector (normal)
        Vector2 normal = { -edge.y, edge.x };
        
        // Normalize
        axes.push_back(Vector2Normalize(normal));
    }
    
    return axes;
}

std::vector<Vector2> HitBox::getWorldVertices() const {
    std::vector<Vector2> worldVertices;
    
    for (const Vector2& vertex : vertices) {
        // First rotate around origin
        Vector2 rotated = Vector2Rotate(vertex, rotation * DEG2RAD);
        // Then translate to world position
        Vector2 worldVertex = Vector2Add(rotated, position);
        worldVertices.push_back(worldVertex);
    }
    
    return worldVertices;
}

bool HitBox::hasOverlap(const HitBox& shape1, const HitBox& shape2, Vector2 axis) {
    ProjectionRange proj1 = projectOntoAxis(shape1, axis);
    ProjectionRange proj2 = projectOntoAxis(shape2, axis);
    
    return proj1.max >= proj2.min && proj2.max >= proj1.min;
}

HitBox::ProjectionRange HitBox::projectOntoAxis(const HitBox& shape, Vector2 axis) {
    std::vector<Vector2> worldVertices = shape.getWorldVertices();
    
    float min = Vector2DotProduct(worldVertices[0], axis);
    float max = min;
    
    for (size_t i = 1; i < worldVertices.size(); i++) {
        float projection = Vector2DotProduct(worldVertices[i], axis);
        min = fminf(min, projection);
        max = fmaxf(max, projection);
    }
    
    return { min, max };
}

void HitBox::updatePosition(Vector2 move) { position += move; }
void HitBox::updateRotation(float rotate) { rotation += rotate; }

void HitBox::renderDebug(Color outlineColor, Color fillColor) const {
    std::vector<Vector2> worldVerts = getWorldVertices();
    
    for (size_t i = 0; i < worldVerts.size(); i++) {
        Vector2 start = worldVerts[i];
        Vector2 end = worldVerts[(i + 1) % worldVerts.size()];
        DrawLineV(start, end, outlineColor);
    }
    
    DrawCircleV(position, 2.0f, RED);
}