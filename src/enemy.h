#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>
#include "AnimationManager.h"
#include "tileMap.h"

class Enemy {
public:
    Enemy(const std::string& enemyName, const std::string& idleAnim, const std::string& hitAnim,
          const std::string& deathAnim, const std::string& moveDownAnim, const std::string& moveRightAnim,
          const std::string& moveLeftAnim, const std::string& moveUpAnim, float spd, float detectRange)
        : sprite(enemyIdleTexture), speed(spd), detectionRange(detectRange), enemyName(enemyName)
    {
        setupAnimation(enemyIdleTexture,      enemyName + "Idle",      idleAnim,      {4, 1}, {32, 32}, {0, 0}, 10);
        setupAnimation(enemyHitTexture,       enemyName + "Hit",       hitAnim,       {4, 4}, {32, 32}, {0, 0},  6);
        setupAnimation(enemyDeathTexture,     enemyName + "Death",     deathAnim,     {9, 1}, {32, 32}, {0, 0},  4);
        setupAnimation(enemyMoveDownTexture,  enemyName + "MoveDown",  moveDownAnim,  {3, 1}, {32, 32}, {0, 0},  7);
        setupAnimation(enemyMoveRightTexture, enemyName + "MoveRight", moveRightAnim, {3, 1}, {32, 32}, {0, 0},  6);
        setupAnimation(enemyMoveLeftTexture,  enemyName + "MoveLeft",  moveLeftAnim,  {3, 1}, {32, 32}, {0, 0},  6);
        setupAnimation(enemyMoveUpTexture,    enemyName + "MoveUp",    moveUpAnim,    {3, 1}, {32, 32}, {0, 0},  7);

        sprite.setTexture(enemyIdleTexture);
        spawnPoint = {100.f, 100.f};
        sprite.setPosition(spawnPoint);
        currentAnimation = enemyName + "Idle";
    }

    virtual ~Enemy() = default;

    virtual void update(const TileMap& map, const sf::Vector2f& playerPosition) {
        float distToPlayer = std::hypot(sprite.getPosition().x - playerPosition.x,
                                        sprite.getPosition().y - playerPosition.y);

        if (distToPlayer < detectionRange) {
            sf::Vector2f dir = playerPosition - sprite.getPosition();
            dir /= std::hypot(dir.x, dir.y);
            direction = dir * speed;
        } else {
            float distToSpawn = std::hypot(sprite.getPosition().x - spawnPoint.x,
                                           sprite.getPosition().y - spawnPoint.y);
            if (distToSpawn > 3.f * tileSize || distToSpawn < tileSize) {
                direction = {static_cast<float>((std::rand() % 3) - 1),
                             static_cast<float>((std::rand() % 3) - 1)};
                float len = std::hypot(direction.x, direction.y);
                if (len > 0.f)
                    direction /= len;
                direction *= speed;
            }
        }

        sf::Vector2f newPosition = sprite.getPosition() + direction;

        if (map.isCollision({newPosition.x, sprite.getPosition().y}, {32, 32}))
            direction.x = 0.f;
        if (map.isCollision({sprite.getPosition().x, newPosition.y}, {32, 32}))
            direction.y = 0.f;

        sprite.setPosition(sprite.getPosition() + direction);

        if      (direction.x > 0.f) currentAnimation = enemyName + "MoveRight";
        else if (direction.x < 0.f) currentAnimation = enemyName + "MoveLeft";
        else if (direction.y > 0.f) currentAnimation = enemyName + "MoveDown";
        else if (direction.y < 0.f) currentAnimation = enemyName + "MoveUp";

        AnimationManager::update(currentAnimation, sprite);
    }

    void setPosition(const sf::Vector2f& position) { sprite.setPosition(position); }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

protected:
    sf::Sprite    sprite;
    sf::Texture   enemyIdleTexture;
    sf::Texture   enemyHitTexture;
    sf::Texture   enemyDeathTexture;
    sf::Texture   enemyMoveDownTexture;
    sf::Texture   enemyMoveRightTexture;
    sf::Texture   enemyMoveLeftTexture;
    sf::Texture   enemyMoveUpTexture;
    std::string   currentAnimation;
    std::string   enemyName;
    sf::Vector2f  spawnPoint;
    sf::Vector2f  direction;
    float         speed;
    float         tileSize     = 32.f;
    float         detectionRange;

    static void setupAnimation(sf::Texture& texture, const std::string& animationName,
                               const std::string& filePath, sf::Vector2i frameCount,
                               sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency) {
        if (!texture.loadFromFile(filePath))
            std::cerr << "Failed to load texture: " << filePath << "\n";
        else
            AnimationManager::addAnimation(animationName, texture, frameCount, frameSize, startPosition, frequency);
    }
};

class BatEnemy : public Enemy {
public:
    BatEnemy()
        : Enemy("Bat",
                "assets/enemy/bat/BatFlyIdle.png",
                "assets/enemy/bat/BatAttack.png",
                "assets/enemy/bat/BatDie.png",
                "assets/enemy/bat/BatFlyIdle.png",
                "assets/enemy/bat/BatFlyIdle.png",
                "assets/enemy/bat/BatFlyIdle.png",
                "assets/enemy/bat/BatFlyIdle.png",
                0.6f, 100.f) {}
};
