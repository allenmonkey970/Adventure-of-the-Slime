#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <set>

const std::set<int> collidableTiles = {2, 3}; // Add any tile numbers that should be collidable

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap()
        : m_width(0), m_height(0) {}

    bool load(const std::filesystem::path& tileset, sf::Vector2u tileSize, const std::vector<int>& tiles, unsigned int width, unsigned int height)
{
    // Load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
    {
        std::cerr << "Failed to load tileset texture from file: " << tileset << "\n";
        return false;
    }

    // Resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 6);

    // Resize the collision array to fit the level size
    m_collision.resize(width * height, false);

    // Populate the vertex array, with two triangles per tile
    for (std::size_t i = 0; i < width; ++i)
    {
        for (std::size_t j = 0; j < height; ++j)
        {
            // Get the current tile number
            const int tileNumber = tiles[static_cast<std::vector<int>::size_type>(i + j * static_cast<std::size_t>(width))];

            // Find its position in the tileset texture
            const int tu = tileNumber % static_cast<int>(m_tileset.getSize().x / tileSize.x);
            const int tv = tileNumber / static_cast<int>(m_tileset.getSize().x / tileSize.x);

            // Get a pointer to the triangles' vertices of the current tile
            sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

            // Define the 6 corners of the two triangles
            triangles[0].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>(j * tileSize.y));
            triangles[1].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>(j * tileSize.y));
            triangles[2].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>((j + 1) * tileSize.y));
            triangles[3].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>((j + 1) * tileSize.y));
            triangles[4].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>(j * tileSize.y));
            triangles[5].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>((j + 1) * tileSize.y));

            // Define the 6 matching texture coordinates
            triangles[0].texCoords = sf::Vector2f(static_cast<float>(tu * tileSize.x), static_cast<float>(tv * tileSize.y));
            triangles[1].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * tileSize.x), static_cast<float>(tv * tileSize.y));
            triangles[2].texCoords = sf::Vector2f(static_cast<float>(tu * tileSize.x), static_cast<float>((tv + 1) * tileSize.y));
            triangles[3].texCoords = sf::Vector2f(static_cast<float>(tu * tileSize.x), static_cast<float>((tv + 1) * tileSize.y));
            triangles[4].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * tileSize.x), static_cast<float>(tv * tileSize.y));
            triangles[5].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * tileSize.x), static_cast<float>((tv + 1) * tileSize.y));

            // Set collision for specific tile numbers
            if (collidableTiles.count(tileNumber) > 0) {
                m_collision[static_cast<std::vector<bool>::size_type>(i + j * width)] = true;
            }
        }
    }

    return true;
}

    bool isCollision(const sf::Vector2f& position, sf::Vector2u tileSize) const {
        unsigned int x = static_cast<unsigned int>(position.x) / tileSize.x;
        unsigned int y = static_cast<unsigned int>(position.y) / tileSize.y;

        // Boundary checks
        if (x >= m_width || y >= m_height) {
            return true; // Treat out-of-bounds as collisions
        }

        return m_collision[static_cast<std::vector<bool>::size_type>(x + y * m_width)];
    }



    bool loadFromFile(const std::filesystem::path& mapFile, const std::filesystem::path& tileset, sf::Vector2u tileSize)
    {
        std::ifstream file(mapFile);
        if (!file.is_open())
        {
            std::cerr << "Failed to open map file: " << mapFile << "\n";
            return false;
        }

        std::vector<int> tiles;
        unsigned int width = 0, height = 0;
        std::string line;

        while (std::getline(file, line))
        {
            std::istringstream stream(line);
            int tile;
            while (stream >> tile)
            {
                tiles.push_back(tile);
            }
            if (width == 0)
                width = static_cast<unsigned int>(tiles.size());
            ++height;
        }

        file.close();

        m_width = width;
        m_height = height;

        return load(tileset, tileSize, tiles, width, height);
    }


private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture     m_tileset;
    std::vector<bool> m_collision;
    unsigned int m_width;
    unsigned int m_height;
};

#endif // TILEMAP_H
