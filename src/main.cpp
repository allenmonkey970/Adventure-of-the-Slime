#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "tileMap.h"
#include "slime.h"
#include "icon.h"
#include <iostream>
#include "SoundPlayer.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({1920u, 1080u}), "Adventure of the Slime");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    std::cout << "Window created successfully.\n";
    SoundPlayer soundPlayer;
    std::string filename;
    Icon icon;
    icon.draw(window);
    std::cout << "Icon drawn successfully.\n";

    Slime mainSlime;

    // Create the tilemap from the level definition
    TileMap map;
    if (!map.loadFromFile("assets/map.txt", "assets/tileMap.png", {32, 32})) {
        std::cerr << "Failed to load tile map from file.\n";
        return -1;
    }

    std::cout << "Tile map loaded successfully.\n";

    sf::View view(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));
    view.setCenter(mainSlime.getPosition());

    view.zoom(0.3f);
    sf::Clock clock;
    sf::Clock animationClock;
    constexpr float animationUpdateInterval = 0.2f; // Adjust the interval to control animation speed
    std::string currentAnimation = "idle";

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event && event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Catch the resize events
            if (event && event->is<sf::Event::Resized>()) {
                const auto* resized = event->getIf<sf::Event::Resized>();
                sf::Vector2f newSize(static_cast<float>(resized->size.x), static_cast<float>(resized->size.y));
                view.setSize(newSize); // Update the view size
                view.zoom(0.3f); // Maintain zoom factor
                window.setView(view);
            }
        }

        sf::Vector2f movement(0.f, 0.f);
        constexpr float moveSpeed = 5.f; // Adjust the speed as needed
        bool isMoving = false;

        // Handle continuous keyboard input for movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            movement.x -= moveSpeed;
            if (currentAnimation != "moveLeft") {
                mainSlime.setScale("moveLeft", {1.f, 1.f});
                currentAnimation = "moveLeft";
            }
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            movement.x += moveSpeed;
            if (currentAnimation != "moveRight") {
                mainSlime.setScale("moveRight", {1.f, 1.f});
                currentAnimation = "moveRight";
            }
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            movement.y -= moveSpeed;
            if (currentAnimation != "moveUp") {
                mainSlime.setScale("moveUp", {1.f, 1.f});
                currentAnimation = "moveUp";
            }
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            movement.y += moveSpeed;
            if (currentAnimation != "moveDown") {
                mainSlime.setScale("moveDown", {1.f, 1.f});
                currentAnimation = "moveDown";
            }
            isMoving = true;
        }

        if (!isMoving && currentAnimation != "idle") {
            mainSlime.setScale("idle", {1.f, 1.f});
            currentAnimation = "idle";
        }

        // Check collision
        sf::Vector2f newPos = mainSlime.getPosition() + movement;
        if (!map.isCollision(newPos, {32, 32})) {
            mainSlime.move(movement);
        }

        // Calculate time since last update
        float deltaTime = clock.restart().asSeconds();
        float animationDeltaTime = animationClock.getElapsedTime().asSeconds();

        // Update the animation based on the time interval
        if (animationDeltaTime >= animationUpdateInterval) {
            animationClock.restart();
            mainSlime.updateAnimation();
        }

        // Update view position to follow the player
        view.setCenter(mainSlime.getPosition());
        window.clear();
        window.setView(view);
        window.draw(map);
        mainSlime.draw(window);
        window.display();
    }

    return 0;
}
