#include "Enemy.h"

Enemy::Enemy(const std::vector<Vector2>& vertices, Vector2 position, float rotation,
             float health, float armour, float speed, Texture2D* texture, float damage, float atkSpeed, Vector2 targetPosition)
    : Entity(vertices, position, rotation, health, armour, speed, false, texture),
      damage(damage),
      attackSpeed(atkSpeed),
      targetPosition(targetPosition)
{ }

bool Enemy::takeDamage(Bullet *bullet) {
    
    if (recentBulletHitCooldowns.find(bullet->getId()) != recentBulletHitCooldowns.end()) {
        if (recentBulletHitCooldowns[bullet->getId()] > 0.0f){
            std::cout << "dmg on cooldown" << std::endl;
            return false;
        }
    } 
    // std::cout << "took damage" << std::endl;
    health -= bullet->getDamage();
    if (health <= 0) {
        // std::cout << "Enemy destroyed" << std::endl;
        // GameManager::Instance->removeEnemy(this);
        return true;
    }
    recentBulletHitCooldowns[bullet->getId()] = GameManager::Instance->HIT_COOLDOWN;
    return false;
}

void Enemy::update(float deltaTime) {
    // Handle movement and rotation
    Vector2 moveDirection = Vector2Normalize(Vector2Subtract(targetPosition, getPosition()));
    moveDirection = Vector2Scale(moveDirection, speed * deltaTime);
    move(moveDirection);
    rotation = atan((targetPosition.y - getPosition().y) / (targetPosition.x - getPosition().x)) * RAD2DEG;
    if (targetPosition.x < getPosition().x) {
        rotation += 180;
    }

    // Handle attacking
    atkTimer += deltaTime;
    if (target != nullptr && atkTimer >= attackSpeed) {
        std::cout << "Attacking target" << std::endl;
        target->addHealth(-damage);
        atkTimer = 0.0f;
    }
    
    // Handle hit cooldown
    std::vector<int> idsToRemove;
    for (auto& [id, cooldown] : recentBulletHitCooldowns) {
        recentBulletHitCooldowns[id] -= deltaTime;
        if (recentBulletHitCooldowns[id] <= 0.0f) {
            idsToRemove.push_back(id);
        }
    }

    for (int id : idsToRemove) {
        recentBulletHitCooldowns.erase(id);
    }

}

void Enemy::render() {
    if (texture) {
        Vector2 pos = getPosition();
        Rectangle source = { 0, 0, (float)texture->width, (float)texture->height };
        Rectangle dest = { pos.x, pos.y, (float)texture->width, (float)texture->height };
        Vector2 origin = { texture->width/2.0f, texture->height/2.0f };
        DrawTexturePro(*texture, source, dest, origin, getRotation(), WHITE);
    } else {
        getHitBox().renderDebug(RED, BLUE);
    }
}

