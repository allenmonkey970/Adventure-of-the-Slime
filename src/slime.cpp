#include "slime.h"

sf::Vector2f Slime::handleMovement(const TileMap& map) {
    try {
        sf::Vector2f movement(0.f, 0.f);
        constexpr float horizontalMoveSpeed = 3.f;
        constexpr float verticalMoveSpeed = 5.f;
        bool isMoving = false;

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

        if (!isMoving && currentAnimation != "idle") {
            currentAnimation = "idle";
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