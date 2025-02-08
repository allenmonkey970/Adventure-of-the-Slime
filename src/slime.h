#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationManager.h"
#include <iostream>

class Slime {
public:
    Slime() : sprite(hitTexture) {
        // Load texture and set up animations
        if (!hitTexture.loadFromFile("assets/slime/hit/damage.png")) {
            std::cerr << "Failed to load texture\n";
        } else {
            sprite.setTexture(hitTexture); // Set the texture to the sprite
            AnimationManager::addAnimation("hit", hitTexture, {7, 1}, {40, 32}, {0, 0}, 3);
            sprite.setScale({2, 2});
        }
        if (!idleTexture.loadFromFile("assets/slime/Idle/idle.png")) {
            std::cerr << "Failed to load texture\n";
        } else {
            sprite.setTexture(idleTexture); // Set the texture to the sprite
            AnimationManager::addAnimation("hit", idleTexture, {7, 1}, {30, 27}, {0, 0}, 3);
            sprite.setScale({2, 2});
        }
        if (!deathTexture.loadFromFile("assets/slime/Death/death.png")) {
            std::cerr << "Failed to load texture\n";
        } else {
            sprite.setTexture(deathTexture); // Set the texture to the sprite
            AnimationManager::addAnimation("hit", deathTexture, {6, 1}, {55, 45}, {0, 0}, 2);
            sprite.setScale({2, 2});
        }
    }

    void draw(sf::RenderWindow& window) {
        AnimationManager::update("hit", sprite);
        window.draw(sprite);
    }

private:
    sf::Sprite sprite;
    sf::Texture hitTexture;
    sf::Texture idleTexture;
    sf::Texture deathTexture;
};
