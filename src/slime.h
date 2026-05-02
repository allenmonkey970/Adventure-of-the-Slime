#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "AnimationManager.h"
#include "tileMap.h"

class Slime {
public:
    Slime();
    void draw(sf::RenderWindow& window);
    void move(const sf::Vector2f& offset);
    void hit(const std::string& direction);

    void setPosition(const sf::Vector2f& position) {
        sprite.setPosition(position);
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void updateAnimation() {
        AnimationManager::update(currentAnimation, sprite);
    }

    void handleMovement(const TileMap& map, const sf::RenderWindow& window);

private:
    sf::Sprite  sprite;
    sf::Texture hitRightTexture;
    sf::Texture hitLeftTexture;
    sf::Texture idleTexture;
    sf::Texture deathTexture;
    sf::Texture moveDownTexture;
    sf::Texture moveRightTexture;
    sf::Texture moveLeftTexture;
    sf::Texture moveUpTexture;
    std::string currentAnimation;
    bool        isAttacking = false;

    static void setupAnimation(sf::Texture& texture, const std::string& animationName,
                               const std::string& filePath, sf::Vector2i frameCount,
                               sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency);
};
