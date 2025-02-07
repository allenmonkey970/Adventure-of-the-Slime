#include "AnimationManager.h"
#include <iostream>

// Static member variable definitions
std::map<std::string, sf::Texture> AnimationManager::m_textures;
std::map<std::string, sf::Vector2i> AnimationManager::m_indicies;
std::map<std::string, sf::Vector2i> AnimationManager::m_startingIndicies;
std::map<std::string, sf::Vector2i> AnimationManager::m_endingIndicies;
std::map<std::string, sf::Vector2i> AnimationManager::m_sheetSizes;
std::map<std::string, sf::Vector2i> AnimationManager::m_spriteSizes;
std::map<std::string, int> AnimationManager::m_frequencies;
std::map<std::string, int> AnimationManager::m_timesUpdated;

void AnimationManager::update(const std::string &animation, sf::Sprite &sprite) {
    // Check if the animation entry exists
    if (m_sheetSizes[animation] != sf::Vector2i(0, 0)) {
        // Increment the update counter for the animation
        if (++m_timesUpdated[animation] >= m_frequencies[animation]) {
            // Reset the update counter
            m_timesUpdated[animation] = 0;
            // Calculate the texture rectangle for the current frame
            sf::IntRect rect({
                                 m_indicies[animation].x * m_spriteSizes[animation].x,
                                 m_indicies[animation].y * m_spriteSizes[animation].y
                             },
                             {m_spriteSizes[animation].x, m_spriteSizes[animation].y});
            // Update frame index
            if (++m_indicies[animation].y >= m_sheetSizes[animation].y) {
                m_indicies[animation].y = 0;
                if (++m_indicies[animation].x >= m_sheetSizes[animation].x) {
                    m_indicies[animation].x = 0;
                }
            }
            // Apply texture and texture rectangle to the sprite
            sprite.setTexture(m_textures[animation]);
            sprite.setTextureRect(rect);
        }
    } else {
        // Error handling if the animation entry does not exist
        std::cerr << "No animation entry found for \"" << animation << "\"!" << std::endl;
    }
}

void AnimationManager::updateAll(std::map<std::string, sf::Sprite> &map) {
    // Iterate through all animations in the map and update each one
    for (auto &element: map) {
        update(element.first, element.second);
    }
}

void AnimationManager::addAnimation(const std::string &animation, const sf::Texture &texture,
                                    sf::Vector2i sheetSize, sf::Vector2i spriteSize,
                                    sf::Vector2i index, int frequency,
                                    sf::Vector2i startingIndex) {
    // Add a new animation entry with specified properties
    m_textures[animation] = texture;
    m_sheetSizes[animation] = sheetSize;
    m_spriteSizes[animation] = spriteSize;
    m_indicies[animation] = index;
    m_startingIndicies[animation] = startingIndex;
    m_endingIndicies[animation] = sheetSize;
    m_frequencies[animation] = frequency;
}

void AnimationManager::deleteAnimation(const std::string &animation) {
    // Remove the animation entry from all maps
    m_textures.erase(animation);
    m_indicies.erase(animation);
    m_startingIndicies.erase(animation);
    m_endingIndicies.erase(animation);
    m_sheetSizes.erase(animation);
    m_spriteSizes.erase(animation);
    m_frequencies.erase(animation);
    m_timesUpdated.erase(animation);
}

void AnimationManager::setAnimationFrequency(const std::string &animation, int frequency) {
    // Set the update frequency of the specified animation
    m_frequencies[animation] = frequency;
}

void AnimationManager::setAnimationIndex(const std::string &animation, sf::Vector2i index) {
    // Set the current frame index of the specified animation
    m_indicies[animation] = index;
}

void AnimationManager::setAnimationSheetSize(const std::string &animation, sf::Vector2i size) {
    // Set the sheet size of the specified animation
    m_sheetSizes[animation] = size;
}

void AnimationManager::setAnimationSpriteSize(const std::string &animation, sf::Vector2i size) {
    // Set the sprite size of the specified animation
    m_spriteSizes[animation] = size;
}

void AnimationManager::setAnimationTexture(const std::string &animation, const sf::Texture &texture) {
    // Set the texture of the specified animation
    m_textures[animation] = texture;
}

void AnimationManager::resetAnimationIndex(const std::string &animation) {
    // Reset the current frame index of the specified animation to the starting index
    m_indicies[animation] = m_startingIndicies[animation];
}

void AnimationManager::setAnimationStartingIndex(const std::string &animation, sf::Vector2i index) {
    // Set the starting frame index of the specified animation
    m_startingIndicies[animation] = index;
}

void AnimationManager::setAnimationEndingIndex(const std::string &animation, sf::Vector2i index) {
    // Set the ending frame index of the specified animation
    m_endingIndicies[animation] = index;
}
