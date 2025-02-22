#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <iostream>

#include "Entity/Entity.h"
#include "raylib.h"
#include "raymath.h"

class CameraController {
private:
    Camera2D camera;
    Entity* trackedEntity;
    Vector2 position;
    float zoom;
    float targetZoom;
    float smoothSpeed;
    float zoomSpeed;
    float minZoom;
    float maxZoom;

public:
    CameraController();
    ~CameraController() = default;

    void update(float deltaTime);
    void setTrackedEntity(Entity* entity);
    void setPosition(Vector2 pos);
    
    const Camera2D& getCamera() const { return camera; }
    Vector2 getScreenToWorld(Vector2 screenPos) const;
    Vector2 getWorldToScreen(Vector2 worldPos) const;
};

#endif