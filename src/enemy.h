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
    enemy() : sprite(idleTexture), currentAnimation("idle") {
        setupAnimation(hitTexture, "hit", "assets/enemy/enemyBat/BatAttack.png", {7, 1}, {40, 32}, {0, 0}, 6);
        setupAnimation(idleTexture, "idle", "assets/enemy/enemyBat/BatFlyIdle.png", {7, 1}, {30, 27}, {0, 0}, 10);
        setupAnimation(deathTexture, "death", "assets/enemy/enemyBat/BatDie.png", {6, 1}, {55, 45}, {0, 0}, 4);

        // Movement animations
        setupAnimation(moveDownTexture, "moveDown", "assets/enemy/walking/down/walkingDown.png", {3, 1}, {42, 42}, {0, 0}, 7);
        setupAnimation(moveRightTexture, "moveRight", "assets/enemy/walking/sideways/walkingRight.png", {7, 1}, {30, 28}, {0, 0}, 6);
        setupAnimation(moveLeftTexture, "moveLeft", "assets/enemy/walking/sideways/walkingLeft.png", {7, 1}, {30, 28}, {0, 0}, 6);
        setupAnimation(moveUpTexture, "moveUp", "assets/enemy/walking/up/walkingUpwards.png", {3, 1}, {43, 42}, {0, 0}, 9);
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

    void update() {
        // Walk around spawn point
        float distance = std::sqrt(std::pow(sprite.getPosition().x - spawnPoint.x, 2) + std::pow(sprite.getPosition().y - spawnPoint.y, 2));
        if (distance > 3.0f * tileSize || distance < tileSize) {
            direction = sf::Vector2f(static_cast<float>((std::rand() % 3) - 1) * tileSize, static_cast<float>((std::rand() % 3) - 1) * tileSize);
        }
        sprite.move(direction * speed);

        // Update animation based on direction
        if (direction.x > 0) {
            setTexture("moveRight", {1.0f, 1.0f});
        } else if (direction.x < 0) {
            setTexture("moveLeft", {1.0f, 1.0f});
        } else if (direction.y > 0) {
            setTexture("moveDown", {1.0f, 1.0f});
        } else {
            setTexture("moveUp", {1.0f, 1.0f});
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
