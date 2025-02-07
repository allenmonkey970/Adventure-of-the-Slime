#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationManager.h"
#include <iostream>

class Slime {
public:
    Slime() : sprite(texture) {
        // Load texture and set up animations
        if (!texture.loadFromFile("assets/idle.png")) {
            std::cerr << "Failed to load texture\n";
        } else {
            sprite.setTexture(texture); // Set the texture to the sprite
            AnimationManager::addAnimation("idle", texture, {3, 2}, {21, 19}, {0, 0}, 10);

            sprite.setScale(sf::Vector2f(2.0f, 2.0f));
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
