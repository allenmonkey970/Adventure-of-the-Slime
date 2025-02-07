#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationManager.h"
#include <iostream>

class Slime {
public:
    Slime() : sprite(texture) {
        // Load texture and set up animations
        if (!texture.loadFromFile("assets/slime/hit/damage.png")) {
            std::cerr << "Failed to load texture\n";
        } else {
            sprite.setTexture(texture); // Set the texture to the sprite
            AnimationManager::addAnimation("hit", texture, {7, 1}, {32, 32}, {0, 0}, 3);
            sprite.setScale({2, 2});
        }
    }

    void draw(sf::RenderWindow& window) {
        AnimationManager::update("hit", sprite);
        window.draw(sprite);
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;
};
