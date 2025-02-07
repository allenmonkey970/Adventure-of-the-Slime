#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationManager.h"
#include <iostream>

class Slime {
public:
    Slime() : sprite(texture) {
        // Load texture and set up animations
        if (!texture.loadFromFile("assets/slime/Swallow/swallow.png")) {
            std::cerr << "Failed to load texture\n";
        } else {
            sprite.setTexture(texture); // Set the texture to the sprite
            AnimationManager::addAnimation("swallow", texture, {14, 1}, {80, 72}, {0, 0}, 4);
        }
    }

    void draw(sf::RenderWindow& window) {
        AnimationManager::update("idle", sprite);
        window.draw(sprite);
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;
};
