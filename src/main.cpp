#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include "tileMap.h"
#include "slime.h"
#include "icon.h"
#include "SoundPlayer.h"
#include <iostream>

void switchMap(TileMap& currentMap, const std::filesystem::path& mapFile, const std::filesystem::path& tilesetFile, const std::set<int>& collidableTiles, const std::string& musicFile);
void handleEvents(sf::RenderWindow& window, sf::View& view);
sf::Vector2f handleMovement(Slime& mainSlime, const TileMap& map, std::string& currentAnimation);



sf::Music backgroundMusic;
int main() {
    sf::RenderWindow window(sf::VideoMode({1920u, 1080u}), "Adventure of the Slime");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    Icon icon;
    icon.draw(window);

    TileMap map;
    std::set<int> collidableTiles = {1};

    const std::filesystem::path caveMapFile = "assets/maps/cave_map.txt";
    const std::filesystem::path caveTilesetFile = "assets/cave/tileSet.png";
    const std::string caveMusicFile = "assets/sound effects/caveMusic.mp3";
    switchMap(map, caveMapFile, caveTilesetFile, collidableTiles, caveMusicFile);

    unsigned int spawnRow = 5;
    unsigned int spawnCol = 7;
    sf::Vector2u tileSize(32, 32);
    sf::Vector2f spawnPosition = TileMap::getTilePosition(spawnRow, spawnCol, tileSize);

    Slime mainSlime;
    mainSlime.setPosition(spawnPosition);
    std::cout << "Initial tile map loaded successfully.\n";

    sf::View view(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));
    view.setCenter(mainSlime.getPosition());
    view.zoom(0.3f);

    sf::Clock animationClock;
    constexpr float animationUpdateInterval = 0.2f;
    std::string currentAnimation = "idle";

    bool mapSwitched = false;

    while (window.isOpen()) {
        handleEvents(window, view);

        sf::Vector2f movement = handleMovement(mainSlime, map, currentAnimation); // This is the movement function do not remove

        float animationDeltaTime = animationClock.getElapsedTime().asSeconds();
        if (animationDeltaTime >= animationUpdateInterval) {
            animationClock.restart();
            mainSlime.updateAnimation();
        }

        view.setCenter(mainSlime.getPosition());
        window.clear();
        window.setView(view);
        window.draw(map);
        mainSlime.draw(window);
        window.display();
    }

    return 0;
}

void switchMap(TileMap& currentMap, const std::filesystem::path& mapFile, const std::filesystem::path& tilesetFile, const std::set<int>& collidableTiles, const std::string& musicFile) {
    if (!currentMap.loadFromFile(mapFile, tilesetFile, {32, 32}, collidableTiles)) {
        std::cerr << "Failed to load tile map from file.\n";
    }
    // Stop any currently playing music
    backgroundMusic.stop();
    // Load and play the new music file
    if (!backgroundMusic.openFromFile(musicFile)) {
        std::cerr << "Failed to load music from file: " << musicFile << std::endl;
    } else {
        backgroundMusic.setLoopPoints({sf::milliseconds(500), sf::seconds(4)});
        backgroundMusic.play();
    }
}

void handleEvents(sf::RenderWindow& window, sf::View& view) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event && event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (event && event->is<sf::Event::Resized>()) {
            const auto* resized = event->getIf<sf::Event::Resized>();
            sf::Vector2f newSize(static_cast<float>(resized->size.x), static_cast<float>(resized->size.y));
            view.setSize(newSize);
            view.zoom(0.3f);
            window.setView(view);
        }
    }
}

sf::Vector2f handleMovement(Slime& mainSlime, const TileMap& map, std::string& currentAnimation) {
    sf::Vector2f movement(0.f, 0.f);
    constexpr float horizontalMoveSpeed = 3.f;
    constexpr float verticalMoveSpeed = 5.f;
    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        movement.x -= horizontalMoveSpeed;
        if (currentAnimation != "moveLeft") {
            mainSlime.setScale("moveLeft", {1.f, 1.f});
            currentAnimation = "moveLeft";
        }
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        movement.x += horizontalMoveSpeed;
        if (currentAnimation != "moveRight") {
            mainSlime.setScale("moveRight", {1.f, 1.f});
            currentAnimation = "moveRight";
        }
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        movement.y -= verticalMoveSpeed;
        if (currentAnimation != "moveUp") {
            mainSlime.setScale("moveUp", {1.f, 1.f});
            currentAnimation = "moveUp";
        }
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        movement.y += verticalMoveSpeed;
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

    sf::Vector2f newPos = mainSlime.getPosition() + movement;
    if (!map.isCollision(newPos, {32, 32})) {
        mainSlime.move(movement);
    }

    return movement;
}
