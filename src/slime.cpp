#include "slime.h"
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <cmath>

static constexpr float PI_OVER_4 = 0.785398163f;

// Staff is rendered at this scale relative to the 128×128 source image.
// Result: ~32×32 world units, similar to one tile.
static constexpr float STAFF_SCALE = 0.25f;
static constexpr float STAFF_OFFSET_X = 20.f; // world units from slime center to staff center

// Sword swing: 9-frame strip at 128×256 per frame, scaled to roughly 38×77 world units.
static constexpr float SWORD_SCALE = 0.3f;
static constexpr float SWORD_OFFSET_X = 12.f;

Slime::Slime() : sprite(idleTexture), weaponSprite(staffTexture), currentAnimation("idle") {
    // Body animations — frame sizes verified against actual PNG dimensions.
    setupAnimation(idleTexture,        "idle",        "assets/slime/Idle/idle.png",                        {7, 1}, {29, 27}, {0, 0}, 10);
    setupAnimation(hitRightTexture,    "hitRight",    "assets/slime/hit/damageRight.png",                  {7, 1}, {42, 42}, {0, 0},  5);
    setupAnimation(hitLeftTexture,     "hitLeft",     "assets/slime/hit/damageLeft.png",                   {7, 1}, {42, 42}, {0, 0},  5);
    setupAnimation(damageTakenTexture, "damageTaken", "assets/slime/damage taken/damageTaken.png",         {3, 1}, {19, 25}, {0, 0},  4);
    setupAnimation(deathTexture,       "death",       "assets/slime/Death/death.png",                      {6, 1}, {55, 45}, {0, 0},  6);
    setupAnimation(moveDownTexture,    "moveDown",    "assets/slime/walking/down/walkingDown.png",         {3, 1}, {42, 42}, {0, 0},  7);
    setupAnimation(moveRightTexture,   "moveRight",   "assets/slime/walking/sideways/walkingRight.png",   {7, 1}, {29, 28}, {0, 0},  6);
    setupAnimation(moveLeftTexture,    "moveLeft",    "assets/slime/walking/sideways/walkingLeft.png",    {7, 1}, {29, 28}, {0, 0},  6);
    setupAnimation(moveUpTexture,      "moveUp",      "assets/slime/walking/up/walkingUpwards.png",       {3, 1}, {42, 42}, {0, 0},  7);

    // Sword swing animation (weapon overlay, drawn on a separate sprite).
    setupAnimation(swordTexture, "swordSwing", "assets/slime/sword/swordSwing.png", {9, 1}, {128, 256}, {0, 0}, 3);

    // Staff is a single 128×128 image, not an animation strip.
    if (!staffTexture.loadFromFile("assets/slime/staff/GreenMagicStaff.png"))
        std::cerr << "Failed to load texture: assets/slime/staff/GreenMagicStaff.png\n";

    // Weapon sprite origin sits at its visual center so we can position it
    // relative to the slime center without manual half-size arithmetic.
    weaponSprite.setTexture(staffTexture);
    weaponSprite.setOrigin({64.f, 64.f});
    weaponSprite.setScale({STAFF_SCALE, STAFF_SCALE});

    sprite.setTexture(idleTexture);
    updateSpriteOrigin();
}

// ---------------------------------------------------------------------------

void Slime::setAnimation(const std::string& anim) {
    if (currentAnimation == anim)
        return;
    currentAnimation = anim;
    updateSpriteOrigin();
    AnimationManager::applyFirstFrame(anim, sprite);
}

// Keep the sprite's visual center pinned to getPosition() regardless of which
// animation is playing (idle ~29×27, hit ~42×42, death ~55×45, etc.).
void Slime::updateSpriteOrigin() {
    sf::Vector2i sz = AnimationManager::getSpriteSize(currentAnimation);
    if (sz.x > 0 && sz.y > 0)
        sprite.setOrigin(sf::Vector2f(sz.x / 2.f, sz.y / 2.f));
}

// ---------------------------------------------------------------------------

void Slime::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    if (isAttacking)
        drawWeapon(window);
}

void Slime::drawWeapon(sf::RenderWindow& window) {
    const sf::Vector2f center    = getPosition();
    const float        xDir      = (attackDirection == "right") ? 1.f : -1.f;

    if (currentWeapon == Weapon::Staff) {
        // Staff: single static image, flipped horizontally to face the attack direction.
        weaponSprite.setTexture(staffTexture);
        weaponSprite.setTextureRect(sf::IntRect({0, 0}, {128, 128}));
        weaponSprite.setOrigin({64.f, 64.f});
        weaponSprite.setScale({xDir * STAFF_SCALE, STAFF_SCALE});
        // Position the staff centre slightly in front of and above the slime centre.
        weaponSprite.setPosition(center + sf::Vector2f(xDir * STAFF_OFFSET_X, -10.f));
        window.draw(weaponSprite);

    } else if (currentWeapon == Weapon::Sword) {
        // Sword swing: animated strip, advanced every frame while attacking.
        AnimationManager::update("swordSwing", weaponSprite);
        // AnimationManager sets texture+rect; apply scale/origin/position after.
        weaponSprite.setOrigin(sf::Vector2f(64.f, 128.f)); // centre of the 128×256 frame
        weaponSprite.setScale({xDir * SWORD_SCALE, SWORD_SCALE});
        weaponSprite.setPosition(center + sf::Vector2f(xDir * SWORD_OFFSET_X, 0.f));
        window.draw(weaponSprite);
    }
}

// ---------------------------------------------------------------------------

void Slime::move(const sf::Vector2f& offset) {
    sprite.move(offset);
}

void Slime::takeDamage() {
    setAnimation("damageTaken");
    AnimationManager::resetAnimationIndex("damageTaken");
}

// ---------------------------------------------------------------------------

void Slime::handleMovement(const TileMap& map, const sf::RenderWindow& window) {
    constexpr float horizontalSpeed = 3.f;
    constexpr float verticalSpeed   = 5.f;

    sf::Vector2f movement(0.f, 0.f);
    bool isMoving = false;

    if (!isAttacking) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            movement.x -= horizontalSpeed;
            setAnimation("moveLeft");
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            movement.x += horizontalSpeed;
            setAnimation("moveRight");
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            movement.y -= verticalSpeed;
            setAnimation("moveUp");
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            movement.y += verticalSpeed;
            setAnimation("moveDown");
            isMoving = true;
        }

        if (!isMoving)
            setAnimation("idle");
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2f charPos  = getPosition();
        float angle = std::atan2(mousePos.y - charPos.y, mousePos.x - charPos.x);

        if (angle > -PI_OVER_4 && angle < PI_OVER_4) {
            setAnimation("hitRight");
            attackDirection = "right";
        } else {
            setAnimation("hitLeft");
            attackDirection = "left";
        }

        // Reset sword swing to frame 0 on the first frame of a new attack.
        if (currentWeapon == Weapon::Sword && !isAttacking)
            AnimationManager::resetAnimationIndex("swordSwing");

        isAttacking = true;
    } else {
        isAttacking = false;
    }

    // Sprite origin sits at the visual centre, so subtract half the collision box to
    // convert to a top-left corner before passing to isCollision (which also uses the
    // size value to convert world coords to tile indices — must stay {32,32}).
    constexpr float halfBox = 16.f;
    const sf::Vector2f pos = getPosition();

    if (map.isCollision(pos + sf::Vector2f(movement.x - halfBox, -halfBox), {32, 32}))
        movement.x = 0.f;
    if (map.isCollision(pos + sf::Vector2f(-halfBox, movement.y - halfBox), {32, 32}))
        movement.y = 0.f;

    move(movement);
}

// ---------------------------------------------------------------------------

void Slime::setupAnimation(sf::Texture& texture, const std::string& animationName,
                           const std::string& filePath, sf::Vector2i frameCount,
                           sf::Vector2i frameSize, sf::Vector2i startPosition, int frequency) {
    if (!texture.loadFromFile(filePath))
        std::cerr << "Failed to load texture: " << filePath << "\n";
    else
        AnimationManager::addAnimation(animationName, texture, frameCount, frameSize, startPosition, frequency);
}
