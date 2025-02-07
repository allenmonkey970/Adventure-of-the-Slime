#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationManager.h"
#include <iostream>

class Slime {
public:
    Slime() : sprite(texture) {
        // Load texture and set up animations
        if (!texture.loadFromFile("assets/slime/walking/sideways/0.png")) {
            std::cerr << "Failed to load texture\n";
        } else {
            sprite.setTexture(texture); // Set the texture to the sprite
            AnimationManager::addAnimation("walk", texture, {1, 1}, {19, 15});
        }
    }

    void draw(sf::RenderWindow& window) {
        AnimationManager::update("walk", sprite);
        window.draw(sprite);
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;
};
