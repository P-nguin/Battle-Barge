#include "CameraController.h"

CameraController::CameraController() 
    : trackedEntity(nullptr)
    , position({0, 0})
    , zoom(1.0f)
    , targetZoom(1.0f)
    , smoothSpeed(5.0f)
    , zoomSpeed(0.1f)
    , minZoom(0.5f)
    , maxZoom(2.0f)
{
    camera = {0};  // Zero initialize the struct
    camera.target = {0, 0};
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void CameraController::update(float deltaTime) {
    // Update zoom with mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        targetZoom += wheel * zoomSpeed;
        targetZoom = Clamp(targetZoom, minZoom, maxZoom);
    }

    // Smooth zoom
    zoom = Lerp(zoom, targetZoom, deltaTime * smoothSpeed);
    camera.zoom = zoom;

    // Update position
    Vector2 targetPos;
    if (trackedEntity) {
        targetPos = trackedEntity->getPosition();
    } else {
        targetPos = position;
    }

    // Smooth camera movement
    camera.target.x = Lerp(camera.target.x, targetPos.x, deltaTime * smoothSpeed);
    camera.target.y = Lerp(camera.target.y, targetPos.y, deltaTime * smoothSpeed);
}

void CameraController::setTrackedEntity(Entity* entity) {
    trackedEntity = entity;
    if (entity) {
        position = entity->getPosition();
    }
}

void CameraController::setPosition(Vector2 pos) {
    position = pos;
}

Vector2 CameraController::getScreenToWorld(Vector2 screenPos) const {
    return GetScreenToWorld2D(screenPos, camera);
}

Vector2 CameraController::getWorldToScreen(Vector2 worldPos) const {
    return GetWorldToScreen2D(worldPos, camera);
}