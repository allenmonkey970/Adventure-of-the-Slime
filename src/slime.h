#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "AnimationManager.h"
#include "tileMap.h"

enum class Weapon { Staff, Sword };

class Slime {
public:
    Slime();
    void draw(sf::RenderWindow& window);
    void move(const sf::Vector2f& offset);
    void takeDamage();
    void setWeapon(Weapon w) { currentWeapon = w; }

    void setPosition(const sf::Vector2f& position) { sprite.setPosition(position); }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }

    void updateAnimation() { AnimationManager::update(currentAnimation, sprite); }

    void handleMovement(const TileMap& map, const sf::RenderWindow& window);

private:
    sf::Sprite  sprite;
    sf::Texture idleTexture;
    sf::Texture hitRightTexture;
    sf::Texture hitLeftTexture;
    sf::Texture damageTakenTexture;
    sf::Texture deathTexture;
    sf::Texture moveDownTexture;
    sf::Texture moveRightTexture;
    sf::Texture moveLeftTexture;
    sf::Texture moveUpTexture;

    sf::Texture staffTexture;
    sf::Texture swordTexture;
    sf::Sprite  weaponSprite;

    Weapon      currentWeapon  = Weapon::Staff;
    std::string currentAnimation;
    std::string attackDirection;        // "left" or "right"
    bool        isAttacking    = false;

    void setAnimation(const std::string& anim);
    void updateSpriteOrigin();
    void drawWeapon(sf::RenderWindow& window);

    static void setupAnimation(sf::Texture& texture, const std::string& animationName,
                               const std::string& filePath, sf::Vector2i frameCount,
                               sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency);
};
