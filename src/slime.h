#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationManager.h"
#include <iostream>

class Slime {
public:
    Slime() : sprite(idleTexture), currentAnimation("idle") {
        setupAnimation(hitTexture, "hit", "assets/slime/hit/damage.png", {7, 1}, {40, 32}, {0, 0}, 6);
        setupAnimation(idleTexture, "idle", "assets/slime/Idle/idle.png", {7, 1}, {30, 27}, {0, 0}, 9);
        setupAnimation(deathTexture, "death", "assets/slime/Death/death.png", {6, 1}, {55, 45}, {0, 0}, 4);

        // Movement animations
        setupAnimation(moveDownTexture, "moveDown", "assets/slime/walking/down/walkingDown.png", {3, 1}, {42, 42}, {0, 0}, 5);
        setupAnimation(moveRightTexture, "moveRight", "assets/slime/walking/sideways/WalkingSideways.png", {7, 1}, {30, 28}, {0, 0}, 6);
        setupAnimation(moveLeftTexture, "moveLeft", "assets/slime/walking/sideways/WalkingSideways.png", {7, 1}, {30, 28}, {0, 0}, 6);
        setupAnimation(moveUpTexture, "moveUp", "assets/slime/walking/up/walkingUpwards.png", {3, 1}, {42, 42}, {0, 0}, 6);
        sprite.setTexture(idleTexture);
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
            sprite.setScale({-scale.x, scale.y}); // Flip horizontally for left movement
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
