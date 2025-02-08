#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationManager.h"
#include <iostream>

class Slime {
public:
    Slime() : sprite(idleTexture), currentAnimation("idle") {
        setupAnimation(hitTexture, "assets/slime/hit/damage.png", "hit", {7, 1}, {40, 32}, {0, 0}, 3);
        setupAnimation(idleTexture, "assets/slime/Idle/idle.png", "idle", {7, 1}, {30, 27}, {0, 0}, 3);
        setupAnimation(deathTexture, "assets/slime/Death/death.png", "death", {6, 1}, {55, 45}, {0, 0}, 2);
        sprite.setTexture(idleTexture);
    }

    void setScale(const std::string& animationName, const sf::Vector2f& scale) {
        if (animationName == "hit") {
            sprite.setTexture(hitTexture);
        } else if (animationName == "idle") {
            sprite.setTexture(idleTexture);
        } else if (animationName == "death") {
            sprite.setTexture(deathTexture);
        }
        currentAnimation = animationName;
        AnimationManager::resetAnimationIndex(currentAnimation);
        sprite.setScale(scale);
    }

    void draw(sf::RenderWindow& window) {
        AnimationManager::update(currentAnimation, sprite);
        window.draw(sprite);
    }

    void move(const sf::Vector2f& offset) {
        sprite.move(offset);
    }

private:
    sf::Sprite sprite;
    sf::Texture hitTexture;
    sf::Texture idleTexture;
    sf::Texture deathTexture;
    std::string currentAnimation;

    static void setupAnimation(sf::Texture& texture, const std::string& filePath, const std::string& animationName, sf::Vector2i frameCount, sf::Vector2i frameSize, sf::Vector2i startPosition, int speed) {
        if (!texture.loadFromFile(filePath)) {
            std::cerr << "Failed to load texture: " << filePath << "\n";
        } else {
            AnimationManager::addAnimation(animationName, texture, frameCount, frameSize, startPosition, speed);
        }
    }
};
