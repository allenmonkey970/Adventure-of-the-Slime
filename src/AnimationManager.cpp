#include "AnimationManager.h"
#include <iostream>

// Static member variable definitions
std::map<std::string, sf::Texture> AnimationManager::m_textures;
std::map<std::string, sf::Vector2i> AnimationManager::m_indices;
std::map<std::string, sf::Vector2i> AnimationManager::m_startingIndices;
std::map<std::string, sf::Vector2i> AnimationManager::m_endingIndices;
std::map<std::string, sf::Vector2i> AnimationManager::m_sheetSizes;
std::map<std::string, sf::Vector2i> AnimationManager::m_spriteSizes;
std::map<std::string, int> AnimationManager::m_frequencies;
std::map<std::string, int> AnimationManager::m_timesUpdated;

void AnimationManager::update(const std::string &animation, sf::Sprite &sprite) {
    if (m_sheetSizes[animation] != sf::Vector2i(0, 0)) {
        if (++m_timesUpdated[animation] >= m_frequencies[animation]) {
            m_timesUpdated[animation] = 0;
            sf::IntRect rect({
                                 m_indices[animation].x * m_spriteSizes[animation].x,
                                 m_indices[animation].y * m_spriteSizes[animation].y
                             },
                             {m_spriteSizes[animation].x, m_spriteSizes[animation].y});
            sprite.setTexture(m_textures[animation]);
            sprite.setTextureRect(rect);

            if (m_indices[animation].y < m_sheetSizes[animation].y - 1) {
                ++m_indices[animation].y;
            } else if (m_indices[animation].x < m_sheetSizes[animation].x - 1) {
                m_indices[animation].y = 0;
                ++m_indices[animation].x;
            } else {
                m_indices[animation] = m_startingIndices[animation]; // Reset to starting index for looping animation
            }
        }
    } else {
        std::cerr << "No animation entry found for \"" << animation << "\"!" << std::endl;
    }
}

void AnimationManager::updateAll(std::map<std::string, sf::Sprite> &map) {
    for (auto &element: map) {
        update(element.first, element.second);
    }
}

void AnimationManager::addAnimation(const std::string &animation, const sf::Texture &texture,
                                    sf::Vector2i sheetSize, sf::Vector2i spriteSize,
                                    sf::Vector2i index, int frequency,
                                    sf::Vector2i startingIndex) {
    m_textures[animation] = texture;
    m_sheetSizes[animation] = sheetSize;
    m_spriteSizes[animation] = spriteSize;
    m_indices[animation] = index;
    m_startingIndices[animation] = startingIndex;
    m_endingIndices[animation] = sheetSize;
    m_frequencies[animation] = frequency;
    m_timesUpdated[animation] = 0; // Initialize the times updated counter
}

void AnimationManager::deleteAnimation(const std::string &animation) {
    m_textures.erase(animation);
    m_indices.erase(animation);
    m_startingIndices.erase(animation);
    m_endingIndices.erase(animation);
    m_sheetSizes.erase(animation);
    m_spriteSizes.erase(animation);
    m_frequencies.erase(animation);
    m_timesUpdated.erase(animation);
}

void AnimationManager::setAnimationFrequency(const std::string &animation, int frequency) {
    m_frequencies[animation] = frequency;
}

void AnimationManager::setAnimationSpriteSize(const std::string &animation, sf::Vector2i size) {
    m_spriteSizes[animation] = size;
}

void AnimationManager::setAnimationSheetSize(const std::string &animation, sf::Vector2i size) {
    m_sheetSizes[animation] = size;
}

void AnimationManager::setAnimationIndex(const std::string &animation, sf::Vector2i index) {
    m_indices[animation] = index;
}

void AnimationManager::setAnimationTexture(const std::string &animation, const sf::Texture &texture) {
    m_textures[animation] = texture;
}

void AnimationManager::resetAnimationIndex(const std::string &animation) {
    m_indices[animation] = m_startingIndices[animation];
}

void AnimationManager::setAnimationStartingIndex(const std::string &animation, sf::Vector2i index) {
    m_startingIndices[animation] = index;
}

void AnimationManager::setAnimationEndingIndex(const std::string &animation, sf::Vector2i index) {
    m_endingIndices[animation] = index;
}
