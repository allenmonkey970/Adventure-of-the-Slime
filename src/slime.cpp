#include "slime.h"
#include <SFML/Window/Mouse.hpp>

// Constructor for Slime class
Slime::Slime() : sprite(idleTexture), currentAnimation("idle") {
    setupAnimation(hitRightTexture, "hitRight", "assets/slime/hit/damageRight.png", {7, 1}, {40, 32}, {0, 0}, 6);
    setupAnimation(hitLeftTexture, "hitLeft", "assets/slime/hit/damageRight.png", {7, 1}, {40, 32}, {0, 0}, 6);
    setupAnimation(idleTexture, "idle", "assets/slime/Idle/idle.png", {7, 1}, {30, 27}, {0, 0}, 10);
    setupAnimation(deathTexture, "death", "assets/slime/Death/death.png", {6, 1}, {55, 45}, {0, 0}, 4);

    // Movement animations
    setupAnimation(moveDownTexture, "moveDown", "assets/slime/walking/down/walkingDown.png", {3, 1}, {42, 42}, {0, 0}, 7);
    setupAnimation(moveRightTexture, "moveRight", "assets/slime/walking/sideways/walkingRight.png", {7, 1}, {30, 28}, {0, 0}, 6);
    setupAnimation(moveLeftTexture, "moveLeft", "assets/slime/walking/sideways/walkingLeft.png", {7, 1}, {30, 28}, {0, 0}, 6);
    setupAnimation(moveUpTexture, "moveUp", "assets/slime/walking/up/walkingUpwards.png", {3, 1}, {43, 42}, {0, 0}, 7);
    sprite.setTexture(idleTexture);
}

void Slime::draw(sf::RenderWindow &window) {
    AnimationManager::update(currentAnimation, sprite);
    window.draw(sprite);
}

void Slime::move(const sf::Vector2f &offset) {
    sprite.move(offset);
}

void Slime::hit(const std::string &direction) {
    if (direction == "left") {
        currentAnimation = "hitLeft";
    } else if (direction == "right") {
        currentAnimation = "hitRight";
    }
}

sf::Vector2f Slime::handleMovement(const TileMap& map) {
    try {
        sf::Vector2f movement(0.f, 0.f);
        constexpr float horizontalMoveSpeed = 3.f;
        constexpr float verticalMoveSpeed = 5.f;
        bool isMoving = false;

        // Handle movement only if not attacking
        if (!isAttacking) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                movement.x -= horizontalMoveSpeed;
                if (currentAnimation != "moveLeft") {
                    currentAnimation = "moveLeft";
                }
                isMoving = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                movement.x += horizontalMoveSpeed;
                if (currentAnimation != "moveRight") {
                    currentAnimation = "moveRight";
                }
                isMoving = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                movement.y -= verticalMoveSpeed;
                if (currentAnimation != "moveUp") {
                    currentAnimation = "moveUp";
                }
                isMoving = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                movement.y += verticalMoveSpeed;
                if (currentAnimation != "moveDown") {
                    currentAnimation = "moveDown";
                }
                isMoving = true;
            }

            if (!isMoving && !isAttacking && currentAnimation != "idle") {
                currentAnimation = "idle";
            }
        }

        // Detect left mouse click and trigger hit animation based on the direction of the mouse
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition();
            sf::Vector2f characterPosition = getPosition();
            float angle = atan2(mousePosition.y - characterPosition.y, mousePosition.x - characterPosition.x);

            if (angle > -0.785398163397448309616 && angle < 0.785398163397448309616) {
                hit("right");
            } else if (angle > 0.785398163397448309616 || angle < -0.785398163397448309616) {
                hit("left");
            }
            isAttacking = true;
        } else {
            isAttacking = false;
        }

        sf::Vector2f newPos = getPosition() + movement;
        if (!map.isCollision(newPos, {32, 32})) {
            move(movement);
        }

        return movement;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred while handling movement: " << e.what() << "\n";
        return {0.f, 0.f};
    }
}

void Slime::setTexture(const std::string &animationName, const sf::Vector2f &scale) {
    if (animationName == "hitLeft") {
        sprite.setTexture(hitLeftTexture);
    } else if (animationName == "hitRight") {
        sprite.setTexture(hitRightTexture);
        sprite.setScale(scale);
    } else if (animationName == "idle") {
        sprite.setTexture(idleTexture);
    } else if (animationName == "death") {
        sprite.setTexture(deathTexture);
    } else if (animationName == "moveDown") {
        sprite.setTexture(moveDownTexture);
    } else if (animationName == "moveRight") {
        sprite.setTexture(moveRightTexture);
        sprite.setScale(scale);
    } else if (animationName == "moveLeft") {
        sprite.setTexture(moveLeftTexture);
    } else if (animationName == "moveUp") {
        sprite.setTexture(moveUpTexture);
    }
}

void Slime::setupAnimation(sf::Texture &texture, const std::string &animationName,
                           const std::string &filePath, sf::Vector2i frameCount,
                           sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency) {
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Failed to load texture: " << filePath << "\n";
    } else {
        AnimationManager::addAnimation(animationName, texture, frameCount, frameSize, startPosition, frequency);
    }
}
