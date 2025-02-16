#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <set>
#include <stdexcept>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap() : m_width(0), m_height(0) {}

    bool load(const std::filesystem::path& tileset, sf::Vector2u tileSize, const std::vector<int>& tiles, unsigned int width, unsigned int height, const std::set<int>& collidableTiles)
    {
        if (width == 0 || height == 0)
        {
            std::cerr << "Invalid map dimensions: width and height must be greater than zero.\n";
            return false;
        }

        if (tiles.empty())
        {
            std::cerr << "Empty tiles vector provided.\n";
            return false;
        }

        // Load the tileSet texture
        if (!m_tileset.loadFromFile(tileset))
        {
            std::cerr << "Failed to load tileset texture from file: " << tileset << "\n";
            return false;
        }

        // Resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_vertices.resize(static_cast<std::vector<sf::Vertex>::size_type>(width) * height * 6);

        // Resize the collision array to fit the level size
        m_collision.resize(static_cast<std::vector<bool>::size_type>(width) * height, false);

        // Populate the vertex array, with two triangles per tile
        for (unsigned int i = 0; i < width; ++i)
        {
            for (unsigned int j = 0; j < height; ++j)
            {
                // Get the current tile number
                const int tileNumber = tiles[static_cast<std::vector<int>::size_type>(i + j * static_cast<std::size_t>(width))];

                if (tileNumber < 0)
                {
                    std::cerr << "Invalid tile number at position (" << i << ", " << j << "): " << tileNumber << "\n";
                    return false;
                }

                // Find its position in the tileset texture
                const int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                const int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // Get a pointer to the triangles' vertices of the current tile
                sf::Vertex* triangles = &m_vertices[static_cast<std::vector<sf::Vertex>::size_type>(i + j * static_cast<std::size_t>(width)) * 6];

                // Define the 6 corners of the two triangles
                triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                triangles[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);

                // Define the 6 matching texture coordinates
                triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);

                // Set collision for specific tile numbers
                if (collidableTiles.count(tileNumber) > 0) {
                    m_collision[static_cast<std::vector<bool>::size_type>(i + j * static_cast<std::size_t>(width))] = true;
                }
            }
        }

        return true;
    }

    bool isCollision(const sf::Vector2f& position, sf::Vector2u tileSize) const {
        if (tileSize.x == 0 || tileSize.y == 0)
        {
            throw std::invalid_argument("Tile size cannot be zero.");
        }

        sf::FloatRect playerBounds(position, sf::Vector2f(tileSize.x, tileSize.y));
        for (unsigned int y = position.y / tileSize.y; y < (position.y + tileSize.y) / tileSize.y; ++y) {
            for (unsigned int x = position.x / tileSize.x; x < (position.x + tileSize.x) / tileSize.x; ++x) {
                if (x < m_width && y < m_height) {
                    if (m_collision[static_cast<std::vector<bool>::size_type>(x + y * static_cast<std::size_t>(m_width))]) {
                        sf::FloatRect tileBounds(sf::Vector2f(x * tileSize.x, y * tileSize.y), sf::Vector2f(tileSize.x, tileSize.y));
                        if (playerBounds.findIntersection(tileBounds)) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool loadFromFile(const std::filesystem::path& mapFile, const std::filesystem::path& tileset, sf::Vector2u tileSize, const std::set<int>& collidableTiles)
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
            std::vector<int> row;
            while (stream >> tile)
            {
                if (tile < 0)
                {
                    std::cerr << "Invalid tile value: " << tile << "\n";
                    return false;
                }
                row.push_back(tile);
            }
            if (width == 0)
                width = row.size();
            else if (row.size() != width)
            {
                std::cerr << "Inconsistent row length in map file.\n";
                return false;
            }
            tiles.insert(tiles.end(), row.begin(), row.end());
            ++height;
        }

        file.close();

        if (width == 0 || height == 0)
        {
            std::cerr << "Invalid map dimensions in file: width and height must be greater than zero.\n";
            return false;
        }

        m_width = width;
        m_height = height;

        return load(tileset, tileSize, tiles, width, height, collidableTiles);
    }

    static sf::Vector2f getTilePosition(unsigned int row, unsigned int col, sf::Vector2u tileSize) {
        return {static_cast<float>(col * tileSize.x), static_cast<float>(row * tileSize.y)};
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
    sf::Texture m_tileset;
    std::vector<bool> m_collision;
    unsigned int m_width;
    unsigned int m_height;
};

#endif // TILEMAP_H
