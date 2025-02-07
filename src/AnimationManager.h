#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class AnimationManager {
private:
    // Maps to store animation properties
    static std::map<std::string, sf::Texture> m_textures; // Stores textures for animations
    static std::map<std::string, sf::Vector2i> m_indicies; // Current frame index of the animation
    static std::map<std::string, sf::Vector2i> m_startingIndicies; // Starting frame index of the animation
    static std::map<std::string, sf::Vector2i> m_endingIndicies; // Ending frame index of the animation
    static std::map<std::string, sf::Vector2i> m_sheetSizes; // Size of the animation sheet (in terms of frames)
    static std::map<std::string, sf::Vector2i> m_spriteSizes; // Size of individual sprites
    static std::map<std::string, int> m_frequencies; // Frequency of animation updates
    static std::map<std::string, int> m_timesUpdated; // Times the animation has been updated

public:
    // Updates the specified animation and applies the current frame to the sprite
    static void update(const std::string &animation, sf::Sprite &sprite);

    // Updates all animations in the provided map of sprites
    static void updateAll(std::map<std::string, sf::Sprite> &map);

    // Adds a new animation with the specified properties
    static void addAnimation(const std::string &animation, const sf::Texture &texture, sf::Vector2i sheetSize,
                             sf::Vector2i spriteSize, sf::Vector2i index = {0, 0},
                             int frequency = 0, sf::Vector2i startingIndex = {0, 0});

    // Deletes the specified animation
    static void deleteAnimation(const std::string &animation);

    // Sets the update frequency of the specified animation
    static void setAnimationFrequency(const std::string &animation, int frequency);

    // Sets the sprite size of the specified animation
    static void setAnimationSpriteSize(const std::string &animation, sf::Vector2i size);

    // Sets the sheet size of the specified animation
    static void setAnimationSheetSize(const std::string &animation, sf::Vector2i size);

    // Sets the current frame index of the specified animation
    static void setAnimationIndex(const std::string &animation, sf::Vector2i index);

    // Sets the texture of the specified animation
    static void setAnimationTexture(const std::string &animation, const sf::Texture &texture);

    // Sets the starting frame index of the specified animation
    static void setAnimationStartingIndex(const std::string &animation, sf::Vector2i index);

    // Sets the ending frame index of the specified animation
    static void setAnimationEndingIndex(const std::string &animation, sf::Vector2i index);

    // Resets the current frame index of the specified animation to the starting index
    static void resetAnimationIndex(const std::string &animation);
};
