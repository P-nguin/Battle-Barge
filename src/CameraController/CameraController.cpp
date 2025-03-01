#include "CameraController.h"

CameraController::CameraController() 
    : trackedEntity(nullptr)
    , position({0, 0})
    , targetPosition({0, 0})
    , zoom(1.0f)
    , targetZoom(1.0f)
    , smoothSpeed(5.0f)
    , zoomSpeed(0.1f)
    , minZoom(0.5f)
    , maxZoom(2.0f)
{
    camera = {0};
    camera.target = {0, 0};
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void CameraController::update(float deltaTime) {
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        targetZoom += wheel * zoomSpeed;
        targetZoom = Clamp(targetZoom, minZoom, maxZoom);
    }

    zoom = Lerp(zoom, targetZoom, deltaTime * smoothSpeed);
    camera.zoom = zoom;

    if (trackedEntity) {
        targetPosition = trackedEntity->getPosition();
    }

    position.x = Lerp(position.x, targetPosition.x, deltaTime * smoothSpeed);
    position.y = Lerp(position.y, targetPosition.y, deltaTime * smoothSpeed);
    
    camera.target = position;
}

void CameraController::setTrackedEntity(Entity* entity) {
    trackedEntity = entity;
    if (entity) {
        targetPosition = entity->getPosition();
    }
}

void CameraController::setTargetPosition(Vector2 pos) {
    targetPosition = pos;
}

Vector2 CameraController::getScreenToWorld(Vector2 screenPos) const {
    return GetScreenToWorld2D(screenPos, camera);
}

Vector2 CameraController::getWorldToScreen(Vector2 worldPos) const {
    return GetWorldToScreen2D(worldPos, camera);
}