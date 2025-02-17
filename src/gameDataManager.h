#ifndef GAMEDATAMANAGER_H
#define GAMEDATAMANAGER_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    GameDataManager(const std::string &dataPath) : dataPath(dataPath) {}

    void saveGame(const Slime &mainSlime, const BatEnemy &batEnemy, const TileMap &map,
                  const std::filesystem::path &tilesetFile, sf::Vector2u tileSize, const std::set<int> &collidableTiles) {
        try {
            std::filesystem::create_directories(dataPath + "/AdventureoftheSlime");

            std::ofstream outFile(dataPath + "/AdventureoftheSlime/data.txt");
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
                std::cout << "Data saved to " << dataPath + "/AdventureoftheSlime/data.txt" << std::endl;
            } else {
                throw std::runtime_error("Unable to open file for writing");
            }
        } catch (const std::exception &e) {
            std::cerr << "An error occurred while saving the game: " << e.what() << "\n";
        }
    }

    void loadGame(Slime &mainSlime, BatEnemy &batEnemy, TileMap &map) {
        try {
            std::ifstream inFile(dataPath + "/AdventureoftheSlime/data.txt");
            if (inFile.is_open()) {
                sf::Vector2f slimePos, batEnemyPos;
                std::string mapFilePath, tilesetFilePath;
                sf::Vector2u tileSize;
                size_t collidableTilesSize;
                std::set<int> collidableTiles;

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

                if (mapFilePath.empty() || tilesetFilePath.empty()) {
                    throw std::runtime_error("Map file path or tileset file path is empty");
                }

                mainSlime.setPosition(slimePos);
                batEnemy.setPosition(batEnemyPos);
                map.loadFromFile(mapFilePath, tilesetFilePath, tileSize, collidableTiles);

                inFile.close();
                std::cout << "Game loaded from " << dataPath + "/AdventureoftheSlime/data.txt" << std::endl;
            } else {
                throw std::runtime_error("Unable to open file for reading");
            }
        } catch (const std::exception &e) {
            std::cerr << "An error occurred while loading the game: " << e.what() << "\n";
        }
    }

private:
    std::string dataPath;
};

#endif //GAMEDATAMANAGER_H
