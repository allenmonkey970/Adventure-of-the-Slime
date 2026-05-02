#include "slime.h"
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <cmath>

static constexpr float PI_OVER_4 = 0.785398163f;

Slime::Slime() : sprite(idleTexture), currentAnimation("idle") {
    setupAnimation(hitRightTexture, "hitRight", "assets/slime/hit/damageRight.png",          {7, 1}, {40, 32}, {0, 0},  6);
    setupAnimation(hitLeftTexture,  "hitLeft",  "assets/slime/hit/damageRight.png",          {7, 1}, {40, 32}, {0, 0},  6);
    setupAnimation(idleTexture,     "idle",     "assets/slime/Idle/idle.png",                {7, 1}, {30, 27}, {0, 0}, 10);
    setupAnimation(deathTexture,    "death",    "assets/slime/Death/death.png",              {6, 1}, {55, 45}, {0, 0},  4);
    setupAnimation(moveDownTexture, "moveDown", "assets/slime/walking/down/walkingDown.png", {3, 1}, {42, 42}, {0, 0},  7);
    setupAnimation(moveRightTexture,"moveRight","assets/slime/walking/sideways/walkingRight.png", {7, 1}, {30, 28}, {0, 0}, 6);
    setupAnimation(moveLeftTexture, "moveLeft", "assets/slime/walking/sideways/walkingLeft.png",  {7, 1}, {30, 28}, {0, 0}, 6);
    setupAnimation(moveUpTexture,   "moveUp",   "assets/slime/walking/up/walkingUpwards.png",{3, 1}, {43, 42}, {0, 0},  7);
    sprite.setTexture(idleTexture);
}

void Slime::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Slime::move(const sf::Vector2f& offset) {
    sprite.move(offset);
}

void Slime::hit(const std::string& direction) {
    if (direction == "left")
        currentAnimation = "hitLeft";
    else if (direction == "right")
        currentAnimation = "hitRight";
}

void Slime::handleMovement(const TileMap& map, const sf::RenderWindow& window) {
    constexpr float horizontalSpeed = 3.f;
    constexpr float verticalSpeed   = 5.f;

    sf::Vector2f movement(0.f, 0.f);
    bool isMoving = false;

    if (!isAttacking) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            movement.x -= horizontalSpeed;
            currentAnimation = "moveLeft";
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            movement.x += horizontalSpeed;
            currentAnimation = "moveRight";
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            movement.y -= verticalSpeed;
            currentAnimation = "moveUp";
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            movement.y += verticalSpeed;
            currentAnimation = "moveDown";
            isMoving = true;
        }

        if (!isMoving)
            currentAnimation = "idle";
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2f mousePos   = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2f charPos    = getPosition();
        float angle = std::atan2(mousePos.y - charPos.y, mousePos.x - charPos.x);

        if (angle > -PI_OVER_4 && angle < PI_OVER_4)
            hit("right");
        else
            hit("left");

        isAttacking = true;
    } else {
        isAttacking = false;
    }

    if (map.isCollision(getPosition() + sf::Vector2f(movement.x, 0.f), {32, 32}))
        movement.x = 0.f;
    if (map.isCollision(getPosition() + sf::Vector2f(0.f, movement.y), {32, 32}))
        movement.y = 0.f;
    move(movement);
}

void Slime::setupAnimation(sf::Texture& texture, const std::string& animationName,
                           const std::string& filePath, sf::Vector2i frameCount,
                           sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency) {
    if (!texture.loadFromFile(filePath))
        std::cerr << "Failed to load texture: " << filePath << "\n";
    else
        AnimationManager::addAnimation(animationName, texture, frameCount, frameSize, startPosition, frequency);
}
