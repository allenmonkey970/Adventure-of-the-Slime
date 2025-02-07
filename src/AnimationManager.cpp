#include "AnimationManager.h"
#include <iostream>

std::map<std::string, sf::Texture> AnimationManager::m_textures;
std::map<std::string, sf::Vector2i> AnimationManager::m_indicies;
std::map<std::string, sf::Vector2i> AnimationManager::m_startingIndicies;
std::map<std::string, sf::Vector2i> AnimationManager::m_endingIndicies;
std::map<std::string, sf::Vector2i> AnimationManager::m_sheetSizes;
std::map<std::string, sf::Vector2i> AnimationManager::m_spriteSizes;
std::map<std::string, int> AnimationManager::m_frequencies;
std::map<std::string, int> AnimationManager::m_timesUpdated;

void AnimationManager::update(const std::string& animation, sf::Sprite& sprite) {
    if (m_sheetSizes[animation] != sf::Vector2i(0, 0)) {
        sf::IntRect rect({m_indicies[animation].x * m_spriteSizes[animation].x,
                          m_indicies[animation].y * m_spriteSizes[animation].y},
                         {m_spriteSizes[animation].x, m_spriteSizes[animation].y});
        if (++m_indicies[animation].y >= m_sheetSizes[animation].y) {
            m_indicies[animation].y = 0;
            if (++m_indicies[animation].x >= m_sheetSizes[animation].x) {
                m_indicies[animation].x = 0;
            }
        }
        sprite.setTexture(m_textures[animation]);
        sprite.setTextureRect(rect);
    } else {
        std::cerr << "No animation entry found for \"" << animation << "\"!" << std::endl;
    }
}

void AnimationManager::updateAll(std::map<std::string, sf::Sprite>& map) {
    for (auto& element : map) {
        update(element.first, element.second);
    }
}

void AnimationManager::addAnimation(const std::string& animation, const sf::Texture& texture,
                                    sf::Vector2i sheetSize, sf::Vector2i spriteSize,
                                    sf::Vector2i index, int frequency,
                                    sf::Vector2i startingIndex) {
    m_textures[animation] = texture;
    m_sheetSizes[animation] = sheetSize;
    m_spriteSizes[animation] = spriteSize;
    m_indicies[animation] = index;
    m_startingIndicies[animation] = startingIndex;
    m_endingIndicies[animation] = sheetSize;
    m_frequencies[animation] = frequency;
}

void AnimationManager::deleteAnimation(const std::string& animation) {
    m_textures.erase(animation);
    m_indicies.erase(animation);
    m_startingIndicies.erase(animation);
    m_endingIndicies.erase(animation);
    m_sheetSizes.erase(animation);
    m_spriteSizes.erase(animation);
    m_frequencies.erase(animation);
    m_timesUpdated.erase(animation);
}

void AnimationManager::setAnimationFrequency(const std::string& animation, int frequency) {
    m_frequencies[animation] = frequency;
}

void AnimationManager::setAnimationIndex(const std::string& animation, sf::Vector2i index) {
    m_indicies[animation] = index;
}

void AnimationManager::setAnimationSheetSize(const std::string& animation, sf::Vector2i size) {
    m_sheetSizes[animation] = size;
}

void AnimationManager::setAnimationSpriteSize(const std::string& animation, sf::Vector2i size) {
    m_spriteSizes[animation] = size;
}

void AnimationManager::setAnimationTexture(const std::string& animation, const sf::Texture& texture) {
    m_textures[animation] = texture;
}

void AnimationManager::resetAnimationIndex(const std::string& animation) {
    m_indicies[animation] = m_startingIndicies[animation];
}

void AnimationManager::setAnimationStartingIndex(const std::string& animation, sf::Vector2i index) {
    m_startingIndicies[animation] = index;
}

void AnimationManager::setAnimationEndingIndex(const std::string& animation, sf::Vector2i index) {
    m_endingIndicies[animation] = index;
}
