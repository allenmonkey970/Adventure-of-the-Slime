#pragma once
#ifndef slime_H
#define slime_H
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "AnimationManager.h"
#include "tileMap.h"
#include <iostream>
#include <cmath>

class Slime {
public:
    Slime();
    void draw(sf::RenderWindow &window);
    void move(const sf::Vector2f &offset);
    void hit(const std::string &direction);

    void setPosition(const sf::Vector2f &position) {
        sprite.setPosition(position);
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void updateAnimation() {
        AnimationManager::update(currentAnimation, sprite);
    }

    sf::Vector2f handleMovement(const TileMap &map);

private:
    sf::Sprite sprite;
    sf::Texture hitRightTexture;
    sf::Texture hitLeftTexture;
    sf::Texture idleTexture;
    sf::Texture deathTexture;
    sf::Texture moveDownTexture;
    sf::Texture moveRightTexture;
    sf::Texture moveLeftTexture;
    sf::Texture moveUpTexture;
    std::string currentAnimation;
    bool isAttacking = false;

    void setTexture(const std::string &animationName, const sf::Vector2f &scale);
    static void setupAnimation(sf::Texture &texture, const std::string &animationName,
                               const std::string &filePath, sf::Vector2i frameCount,
                               sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency);
};
#endif
