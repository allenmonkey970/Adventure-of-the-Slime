#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <set>
#include <fstream>
#include <iostream>
#include "slime.h"
#include "enemy.h"
#include "tileMap.h"
#include "utils.h"

class GameDataManager {
public:
    explicit GameDataManager(std::string dataPath) : dataPath(std::move(dataPath)) {}

    void saveGame(const Slime& mainSlime, const BatEnemy& batEnemy, const TileMap& map,
                  const std::filesystem::path& tilesetFile, sf::Vector2u tileSize,
                  const std::set<int>& collidableTiles) {
        try {
            std::filesystem::path savePath(dataPath + "/AdventureoftheSlime");
            std::filesystem::create_directories(savePath);

            std::ofstream out(savePath / "data.txt");
            if (!out.is_open())
                throw std::runtime_error("Unable to open save file for writing.");

            out << mainSlime.getPosition().x  << " " << mainSlime.getPosition().y  << "\n";
            out << batEnemy.getPosition().x   << " " << batEnemy.getPosition().y   << "\n";
            out << map.getFilePath()          << "\n";
            out << tilesetFile.string()       << "\n";
            out << tileSize.x << " " << tileSize.y << "\n";
            out << collidableTiles.size()     << "\n";
            for (int tile : collidableTiles)
                out << tile << " ";
            out << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Failed to save game: " << e.what() << "\n";
        }
    }

    void loadGame(Slime& mainSlime, BatEnemy& batEnemy, TileMap& map) {
        std::filesystem::path savePath(dataPath + "/AdventureoftheSlime/data.txt");
        if (!std::filesystem::exists(savePath))
            return;

        try {
            std::ifstream in(savePath);
            if (!in.is_open())
                throw std::runtime_error("Unable to open save file for reading.");

            sf::Vector2f slimePos, batPos;
            std::string mapFilePath, tilesetFilePath;
            sf::Vector2u tileSize;
            size_t collidableTilesCount = 0;
            std::set<int> collidableTiles;

            in >> slimePos.x >> slimePos.y;
            in >> batPos.x   >> batPos.y;
            in >> mapFilePath;
            in >> tilesetFilePath;
            in >> tileSize.x >> tileSize.y;
            in >> collidableTilesCount;
            for (size_t i = 0; i < collidableTilesCount; ++i) {
                int tile;
                in >> tile;
                collidableTiles.insert(tile);
            }

            if (mapFilePath.empty() || tilesetFilePath.empty())
                throw std::runtime_error("Save file contains empty file paths.");

            mainSlime.setPosition(slimePos);
            batEnemy.setPosition(batPos);
            map.loadFromFile(mapFilePath, tilesetFilePath, tileSize, collidableTiles);
        } catch (const std::exception& e) {
            std::cerr << "Failed to load game: " << e.what() << "\n";
        }
    }

private:
    std::string dataPath;
};
