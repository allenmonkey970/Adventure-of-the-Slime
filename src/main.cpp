#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tileMap.h"
#include "slime.h"
#include "icon.h"
#include "SoundPlayer.h"
#include "enemy.h"
#include "utils.h"
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <set>
#include <fstream>

// Function prototypes
void switchMap(TileMap &currentMap, const std::filesystem::path &mapFile, const std::filesystem::path &tilesetFile,
               const std::set<int> &collidableTiles, const std::string &musicFile);

void handleEvents(sf::RenderWindow &window, sf::View &view);

sf::Vector2f handleMovement(Slime &mainSlime, const TileMap &map, std::string &currentAnimation);

void saveGame(const Slime &mainSlime, const BatEnemy &batEnemy, const TileMap &map,
              const std::filesystem::path &tilesetFile, sf::Vector2u tileSize, const std::set<int> &collidableTiles);

void loadGame(Slime &mainSlime, BatEnemy &batEnemy, TileMap &map);

sf::Music backgroundMusic;

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode({1920u, 1080u}), "Adventure of the Slime");
        window.setFramerateLimit(60);
        window.setVerticalSyncEnabled(true);

        Icon icon;
        icon.draw(window);

        TileMap map;
        std::set<int> collidableTiles = {0};

        const std::filesystem::path caveMapFile = "assets/maps/cave_map.txt";
        const std::filesystem::path caveTilesetFile = "assets/cave/tileSet.png";
        const std::string caveMusicFile = "assets/sound effects/caveMusic.mp3";

        if (!std::filesystem::exists(caveMapFile)) {
            throw std::runtime_error("Map file does not exist: " + caveMapFile.string());
        }

        switchMap(map, caveMapFile, caveTilesetFile, collidableTiles, caveMusicFile);

        unsigned int spawnRow = 10;
        unsigned int spawnCol = 14;
        sf::Vector2u tileSize(32, 32);
        sf::Vector2f spawnPosition = TileMap::getTilePosition(spawnRow, spawnCol, tileSize);

        Slime mainSlime;
        mainSlime.setPosition(spawnPosition);

        BatEnemy batEnemy;
        batEnemy.setPosition(spawnPosition + sf::Vector2f(100.f, 0.f));

        sf::View view(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));
        view.setCenter(mainSlime.getPosition());
        view.zoom(0.25f);

        // Load the game state
        loadGame(mainSlime, batEnemy, map);

        sf::Clock animationClock;
        constexpr float animationUpdateInterval = 0.2f;
        std::string currentAnimation = "idle";

        while (window.isOpen()) {
            handleEvents(window, view);

            sf::Vector2f movement = mainSlime.handleMovement(map);

            float animationDeltaTime = animationClock.getElapsedTime().asSeconds();
            if (animationDeltaTime >= animationUpdateInterval) {
                animationClock.restart();
                mainSlime.updateAnimation();
            }
            batEnemy.update(map, mainSlime.getPosition());
            view.setCenter(mainSlime.getPosition());
            window.clear();
            window.setView(view);
            window.draw(map);
            mainSlime.draw(window);
            batEnemy.draw(window);
            window.display();
        }

        saveGame(mainSlime, batEnemy, map, caveTilesetFile, sf::Vector2u(32, 32), collidableTiles);
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << "\n";
    }

    return 0;
}

void switchMap(TileMap &currentMap, const std::filesystem::path &mapFile, const std::filesystem::path &tilesetFile,
               const std::set<int> &collidableTiles, const std::string &musicFile) {
    try {
        if (!currentMap.loadFromFile(mapFile, tilesetFile, {32, 32}, collidableTiles)) {
            throw std::runtime_error("Failed to load tile map from file.");
        }

        // Stop any currently playing music
        backgroundMusic.stop();

        // Load and play the new music file
        if (!backgroundMusic.openFromFile(musicFile)) {
            throw std::runtime_error("Failed to load music from file: " + musicFile);
        } else {
            backgroundMusic.setLoopPoints({sf::milliseconds(500), sf::seconds(4)});
            backgroundMusic.play();
        }
    } catch (const std::exception &e) {
        std::cerr << "An error occurred in switchMap: " << e.what() << "\n";
    }
}

void handleEvents(sf::RenderWindow &window, sf::View &view) {
    try {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event && event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event && event->is<sf::Event::Resized>()) {
                const auto *resized = event->getIf<sf::Event::Resized>();
                sf::Vector2f newSize(static_cast<float>(resized->size.x), static_cast<float>(resized->size.y));
                view.setSize(newSize);
                view.zoom(0.3f);
                window.setView(view);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "An error occurred while handling events: " << e.what() << "\n";
    }
}

void saveGame(const Slime &mainSlime, const BatEnemy &batEnemy, const TileMap &map, const std::filesystem::path &tilesetFile, sf::Vector2u tileSize, const std::set<int> &collidableTiles) {
    std::string path = GetLocalPath() + "/AdventureoftheSlime/data.txt";
    try {
        std::filesystem::create_directories(GetLocalPath() + "/AdventureoftheSlime"); // Ensure the directory is created

        std::ofstream outFile(path);
        if (outFile.is_open()) {
            outFile << mainSlime.getPosition().x << " " << mainSlime.getPosition().y << "\n";
            outFile << batEnemy.getPosition().x << " " << batEnemy.getPosition().y << "\n";
            outFile << map.getFilePath() << "\n";
            outFile << tilesetFile.string() << "\n";
            outFile << tileSize.x << " " << tileSize.y << "\n";
            outFile << collidableTiles.size() << "\n";
            for (int tile : collidableTiles) {
                outFile << tile << " ";
            }
            outFile << "\n";
            outFile.close();
            std::cout << "Data saved to " << path << std::endl;
        } else {
            throw std::runtime_error("Unable to open file for writing");
        }
    } catch (const std::exception &e) {
        std::cerr << "An error occurred while saving the game: " << e.what() << "\n";
    }
}

void loadGame(Slime &mainSlime, BatEnemy &batEnemy, TileMap &map) {
    std::string path = GetLocalPath() + "/AdventureoftheSlime/data.txt";
    try {
        std::ifstream inFile(path);
        if (inFile.is_open()) {
            sf::Vector2f slimePos, batEnemyPos;
            std::string mapFilePath, tilesetFilePath;
            sf::Vector2u tileSize;
            size_t collidableTilesSize;
            std::set<int> collidableTiles;

            // Read and parse the data
            inFile >> slimePos.x >> slimePos.y;
            inFile >> batEnemyPos.x >> batEnemyPos.y;
            inFile >> mapFilePath;
            inFile >> tilesetFilePath;
            inFile >> tileSize.x >> tileSize.y;
            inFile >> collidableTilesSize;
            for (size_t i = 0; i < collidableTilesSize; ++i) {
                int tile;
                inFile >> tile;
                collidableTiles.insert(tile);
            }

            // Debug statements to print the paths
            std::cout << "Map file path: " << mapFilePath << "\n";
            std::cout << "Tileset file path: " << tilesetFilePath << "\n";

            if (mapFilePath.empty() || tilesetFilePath.empty()) {
                throw std::runtime_error("Map file path or tileset file path is empty");
            }

            mainSlime.setPosition(slimePos);
            batEnemy.setPosition(batEnemyPos);
            map.loadFromFile(mapFilePath, tilesetFilePath, tileSize, collidableTiles);

            inFile.close();
            std::cout << "Game loaded from " << path << std::endl;
        } else {
            throw std::runtime_error("Unable to open file for reading");
        }
    } catch (const std::exception &e) {
        std::cerr << "An error occurred while loading the game: " << e.what() << "\n";
    }
}