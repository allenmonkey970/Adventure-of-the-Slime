#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tileMap.h"
#include "slime.h"
#include "icon.h"
#include "enemy.h"
#include "utils.h"
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <set>
#include "gameDataManager.h"

void switchMap(TileMap& currentMap, sf::Music& music,
               const std::filesystem::path& mapFile, const std::filesystem::path& tilesetFile,
               const std::set<int>& collidableTiles, const std::string& musicFile) {
    if (!currentMap.loadFromFile(mapFile, tilesetFile, {32, 32}, collidableTiles))
        throw std::runtime_error("Failed to load tile map: " + mapFile.string());

    music.stop();
    if (!music.openFromFile(musicFile))
        throw std::runtime_error("Failed to load music: " + musicFile);

    music.setLoopPoints({sf::milliseconds(500), sf::seconds(4)});
    music.play();
}

void handleEvents(sf::RenderWindow& window) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode({1920u, 1080u}), "Adventure of the Slime", sf::State::Fullscreen);
        window.setFramerateLimit(60);

        Icon icon;
        icon.setIcon(window);

        sf::Music backgroundMusic;
        TileMap map;
        const std::set<int> collidableTiles = {0};

        const std::filesystem::path caveMapFile     = "assets/maps/cave_map.txt";
        const std::filesystem::path caveTilesetFile = "assets/cave/tileSet.png";
        const std::string           caveMusicFile   = "assets/sound effects/caveMusic.mp3";

        if (!std::filesystem::exists(caveMapFile))
            throw std::runtime_error("Map file does not exist: " + caveMapFile.string());

        switchMap(map, backgroundMusic, caveMapFile, caveTilesetFile, collidableTiles, caveMusicFile);

        constexpr unsigned int spawnRow = 10;
        constexpr unsigned int spawnCol = 14;
        const sf::Vector2u     tileSize(32, 32);
        // getTilePosition returns the tile's top-left corner; offset by half a tile so the
        // slime's visual centre (its sprite origin) lands in the middle of the spawn tile.
        sf::Vector2f spawnPosition = TileMap::getTilePosition(spawnRow, spawnCol, tileSize)
                                     + sf::Vector2f(tileSize.x / 2.f, tileSize.y / 2.f);

        Slime mainSlime;
        mainSlime.setPosition(spawnPosition);

        BatEnemy batEnemy;
        batEnemy.setPosition(spawnPosition + sf::Vector2f(100.f, 0.f));

        sf::View view(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));
        view.setCenter(mainSlime.getPosition());
        view.zoom(0.2f);

        GameDataManager gameDataManager(GetLocalPath());
        gameDataManager.loadGame(mainSlime, batEnemy, map);

        while (window.isOpen()) {
            handleEvents(window);

            mainSlime.handleMovement(map, window);
            batEnemy.update(map, mainSlime.getPosition());
            mainSlime.updateAnimation();

            view.setCenter(mainSlime.getPosition());

            window.clear();
            window.setView(view);
            window.draw(map);
            mainSlime.draw(window);
            batEnemy.draw(window);
            window.display();
        }

        gameDataManager.saveGame(mainSlime, batEnemy, map, caveTilesetFile, tileSize, collidableTiles);
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
