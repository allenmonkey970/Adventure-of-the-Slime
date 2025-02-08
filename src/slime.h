#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationManager.h"
#include <iostream>

class Slime {
public:
    Slime() : sprite(hitTexture) {
        setupAnimation(hitTexture, "assets/slime/hit/damage.png", "hit", {7, 1}, {40, 32}, {0, 0}, 3);
        setupAnimation(idleTexture, "assets/slime/Idle/idle.png", "idle", {7, 1}, {30, 27}, {0, 0}, 3);
        setupAnimation(deathTexture, "assets/slime/Death/death.png", "death", {6, 1}, {55, 45}, {0, 0}, 2);
    }

    void draw(sf::RenderWindow& window) {
        AnimationManager::update("damage", sprite); // Update this line as needed to switch between animations
        window.draw(sprite);
    }

private:
    sf::Sprite sprite;
    sf::Texture hitTexture;
    sf::Texture idleTexture;
    sf::Texture deathTexture;

    void setupAnimation(sf::Texture& texture, const std::string& filePath, const std::string& animationName, sf::Vector2i frameCount, sf::Vector2i frameSize, sf::Vector2i startPosition, int speed) {
        if (!texture.loadFromFile(filePath)) {
            std::cerr << "Failed to load texture: " << filePath << "\n";
        } else {
            AnimationManager::addAnimation(animationName, texture, frameCount, frameSize, startPosition, speed);
            sprite.setScale({2, 2});
        }
    }
};
