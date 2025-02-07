#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class AnimationManager {
private:
    static std::map<std::string, sf::Texture> m_textures;
    static std::map<std::string, sf::Vector2i> m_indicies;
    static std::map<std::string, sf::Vector2i> m_startingIndicies;
    static std::map<std::string, sf::Vector2i> m_endingIndicies;
    static std::map<std::string, sf::Vector2i> m_sheetSizes;
    static std::map<std::string, sf::Vector2i> m_spriteSizes;
    static std::map<std::string, int> m_frequencies;
    static std::map<std::string, int> m_timesUpdated;

public:
    static void update(const std::string& animation, sf::Sprite& sprite);
    static void updateAll(std::map<std::string, sf::Sprite>& map);
    static void addAnimation(const std::string& animation, const sf::Texture& texture, sf::Vector2i sheetSize,
                             sf::Vector2i spriteSize, sf::Vector2i index = {0, 0},
                             int frequency = 0, sf::Vector2i startingIndex = {0, 0});
    static void deleteAnimation(const std::string& animation);
    static void setAnimationFrequency(const std::string& animation, int frequency);
    static void setAnimationSpriteSize(const std::string& animation, sf::Vector2i size);
    static void setAnimationSheetSize(const std::string& animation, sf::Vector2i size);
    static void setAnimationIndex(const std::string& animation, sf::Vector2i index);
    static void setAnimationTexture(const std::string& animation, const sf::Texture& texture);
    static void setAnimationStartingIndex(const std::string& animation, sf::Vector2i index);
    static void setAnimationEndingIndex(const std::string& animation, sf::Vector2i index);
    static void resetAnimationIndex(const std::string& animation);
};
