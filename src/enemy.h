//
// Created by antho on 2/14/2025.
//

#ifndef ENEMY_H
#define ENEMY_H

#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationManager.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

class enemy {
public:
    enemy() : sprite(idleTexture), currentAnimation("batIdle"), speed(.06f){
        setupAnimation(hitTexture, "batHit", "assets/enemy/bat/BatAttack.png", {4, 4}, {32, 32}, {0, 0}, 6);
        setupAnimation(idleTexture, "batIdle", "assets/enemy/bat/BatFlyIdle.png", {4, 1}, {32, 32}, {0, 0}, 10);
        setupAnimation(deathTexture, "batDeath", "assets/enemy/bat/BatDie.png", {9, 1}, {32, 32}, {0, 0}, 4);

        // Movement animations
        setupAnimation(moveDownTexture, "batMoveDown", "assets/enemy/bat/BatFlyIdle.png", {4, 1}, {32, 32}, {0, 0}, 12);
        setupAnimation(moveRightTexture, "batMoveRight", "assets/enemy/bat/BatFlyIdle.png", {4, 1}, {32, 32}, {0, 0}, 12);
        setupAnimation(moveLeftTexture, "batMoveLeft", "assets/enemy/bat/BatFlyIdle.png", {4, 1}, {32, 32}, {0, 0}, 12);
        setupAnimation(moveUpTexture, "batMoveUp", "assets/enemy/bat/BatFlyIdle.png", {4, 1}, {32, 32}, {0, 0}, 12);
        sprite.setTexture(idleTexture);

        // Initialize random seed
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        spawnPoint = sf::Vector2f(100.0f, 100.0f); // Example spawn point
        sprite.setPosition(spawnPoint);
    }

    void setScale(const std::string& animationName, const sf::Vector2f& scale) {
        setTexture(animationName, scale);
        currentAnimation = animationName;
        AnimationManager::resetAnimationIndex(currentAnimation);
    }

    void draw(sf::RenderWindow& window) {
        AnimationManager::update(currentAnimation, sprite);
        window.draw(sprite);
    }

    void move(const sf::Vector2f& offset) {
        sprite.move(offset);
    }

    void update(const TileMap &map) {
        // Walk around spawn point
        float distance = std::sqrt(std::pow(sprite.getPosition().x - spawnPoint.x, 2) + std::pow(sprite.getPosition().y - spawnPoint.y, 2));
        if (distance > 3.0f * tileSize || distance < tileSize) {
            direction = sf::Vector2f(static_cast<float>((std::rand() % 3) - 1) * tileSize, static_cast<float>((std::rand() % 3) - 1) * tileSize);
        }

        sf::Vector2f newPosition = sprite.getPosition() + direction * speed;

        // Check collision for each direction individually
        bool collision = false;

        sf::Vector2f horizontalMove(newPosition.x, sprite.getPosition().y);
        if (map.isCollision(horizontalMove, {32, 32})) {
            collision = true;
            direction.x = 0; // Cancel horizontal movement
        }

        sf::Vector2f verticalMove(sprite.getPosition().x, newPosition.y);
        if (map.isCollision(verticalMove, {32, 32})) {
            collision = true;
            direction.y = 0; // Cancel vertical movement
        }

        if (!collision) {
            sprite.setPosition(newPosition);
        }

        // Update animation based on direction
        if (direction.x > 0) {
            setTexture("batMoveRight", {1.0f, 1.0f});
        } else if (direction.x < 0) {
            setTexture("batMoveLeft", {1.0f, 1.0f});
        } else if (direction.y > 0) {
            setTexture("batMoveDown", {1.0f, 1.0f});
        } else {
            setTexture("batMoveUp", {1.0f, 1.0f});
        }

        updateAnimation();
    }

    void setPosition(const sf::Vector2f& position) {
        sprite.setPosition(position);
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void updateAnimation() {
        AnimationManager::update(currentAnimation, sprite);
    }

private:
    sf::Sprite sprite;
    sf::Texture hitTexture;
    sf::Texture idleTexture;
    sf::Texture deathTexture;
    sf::Texture moveDownTexture;
    sf::Texture moveRightTexture;
    sf::Texture moveLeftTexture;
    sf::Texture moveUpTexture;
    std::string currentAnimation;
    sf::Vector2f spawnPoint;
    sf::Vector2f direction;
    float speed = 1.0f;
    float tileSize = 32.0f; // Example tile size

    void setTexture(const std::string& animationName, const sf::Vector2f& scale) {
        if (animationName == "hit") {
            sprite.setTexture(hitTexture);
        } else if (animationName == "idle") {
            sprite.setTexture(idleTexture);
        } else if (animationName == "death") {
            sprite.setTexture(deathTexture);
        } else if (animationName == "moveDown") {
            sprite.setTexture(moveDownTexture);
        } else if (animationName == "moveRight") {
            sprite.setTexture(moveRightTexture);
            sprite.setScale(scale);
        } else if (animationName == "moveLeft") {
            sprite.setTexture(moveLeftTexture);
        } else if (animationName == "moveUp") {
            sprite.setTexture(moveUpTexture);
        }
    }

    static void setupAnimation(sf::Texture& texture, const std::string& animationName,
                               const std::string& filePath, sf::Vector2i frameCount,
                               sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency) {
        if (!texture.loadFromFile(filePath)) {
            std::cerr << "Failed to load texture: " << filePath << "\n";
        } else {
            AnimationManager::addAnimation(animationName, texture, frameCount, frameSize, startPosition, frequency);
        }
    }
};




#endif //ENEMY_H
