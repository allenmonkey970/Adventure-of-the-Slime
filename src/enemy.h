#ifndef enemy_H
#define enemy_H
#include <cmath>
class enemy {
public:
    enemy(const std::string& enemyName, const std::string& idleAnim, const std::string& hitAnim, const std::string& deathAnim,
          const std::string& moveDownAnim, const std::string& moveRightAnim, const std::string& moveLeftAnim, const std::string& moveUpAnim,
          float spd, float detectRange)
        : sprite(enemyIdleTexture), speed(spd), detectionRange(detectRange), enemyName(enemyName) { // Ensure sprite is initialized here
        if (!enemyIdleTexture.loadFromFile(idleAnim)) {
            std::cerr << "Failed to load idle texture: " << idleAnim << "\n";
        }
        if (!enemyHitTexture.loadFromFile(hitAnim)) {
            std::cerr << "Failed to load hit texture: " << hitAnim << "\n";
        }
        if (!enemyDeathTexture.loadFromFile(deathAnim)) {
            std::cerr << "Failed to load death texture: " << deathAnim << "\n";
        }
        if (!enemyMoveDownTexture.loadFromFile(moveDownAnim)) {
            std::cerr << "Failed to load move down texture: " << moveDownAnim << "\n";
        }
        if (!enemyMoveRightTexture.loadFromFile(moveRightAnim)) {
            std::cerr << "Failed to load move right texture: " << moveRightAnim << "\n";
        }
        if (!enemyMoveLeftTexture.loadFromFile(moveLeftAnim)) {
            std::cerr << "Failed to load move left texture: " << moveLeftAnim << "\n";
        }
        if (!enemyMoveUpTexture.loadFromFile(moveUpAnim)) {
            std::cerr << "Failed to load move up texture: " << moveUpAnim << "\n";
        }

        setupAnimation(enemyIdleTexture, enemyName + "Idle", idleAnim, {4, 1}, {32, 32}, {0, 0}, 10);
        setupAnimation(enemyHitTexture, enemyName + "Hit", hitAnim, {4, 4}, {32, 32}, {0, 0}, 6);
        setupAnimation(enemyDeathTexture, enemyName + "Death", deathAnim, {9, 1}, {32, 32}, {0, 0}, 4);
        setupAnimation(enemyMoveDownTexture, enemyName + "MoveDown", moveDownAnim, {3, 1}, {32, 32}, {0, 0}, 7);
        setupAnimation(enemyMoveRightTexture, enemyName + "MoveRight", moveRightAnim, {3, 1}, {32, 32}, {0, 0}, 6);
        setupAnimation(enemyMoveLeftTexture, enemyName + "MoveLeft", moveLeftAnim, {3, 1}, {32, 32}, {0, 0}, 6);
        setupAnimation(enemyMoveUpTexture, enemyName + "MoveUp", moveUpAnim, {3, 1}, {32, 32}, {0, 0}, 7);

        // Set initial texture and position
        sprite.setTexture(enemyIdleTexture);
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        spawnPoint = sf::Vector2f(100.0f, 100.0f);
        sprite.setPosition(spawnPoint);
        currentAnimation = enemyName + "Idle";
    }

    virtual void update(const TileMap &map, const sf::Vector2f &playerPosition) {
        float distanceToPlayer = std::sqrt(std::pow(sprite.getPosition().x - playerPosition.x, 2) +
                                           std::pow(sprite.getPosition().y - playerPosition.y, 2));

        if (distanceToPlayer < detectionRange) {
            sf::Vector2f directionToPlayer = playerPosition - sprite.getPosition();
            float length = std::sqrt(directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);
            directionToPlayer /= length;

            direction = directionToPlayer * speed;
        } else {
            float distance = std::sqrt(std::pow(sprite.getPosition().x - spawnPoint.x, 2) +
                                       std::pow(sprite.getPosition().y - spawnPoint.y, 2));
            if (distance > 3.0f * tileSize || distance < tileSize) {
                direction = sf::Vector2f(static_cast<float>((std::rand() % 3) - 1), static_cast<float>((std::rand() % 3) - 1));
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length != 0) {
                    direction /= length;
                }
                direction *= speed;
            }
        }

        sf::Vector2f newPosition = sprite.getPosition() + direction;

        bool collision = false;
        sf::Vector2f horizontalMove(newPosition.x, sprite.getPosition().y);
        if (map.isCollision(horizontalMove, {32, 32})) {
            collision = true;
            direction.x = 0;
        }

        sf::Vector2f verticalMove(sprite.getPosition().x, newPosition.y);
        if (map.isCollision(verticalMove, {32, 32})) {
            collision = true;
            direction.y = 0;
        }

        if (!collision) {
            sprite.setPosition(newPosition);
        }

        if (direction.x > 0) {
            setTexture(enemyName + "MoveRight", {1.0f, 1.0f});
        } else if (direction.x < 0) {
            setTexture(enemyName + "MoveLeft", {1.0f, 1.0f});
        } else if (direction.y > 0) {
            setTexture(enemyName + "MoveDown", {1.0f, 1.0f});
        } else if (direction.y < 0) {
            setTexture(enemyName + "MoveUp", {1.0f, 1.0f});
        }

        updateAnimation();
    }

    void setPosition(const sf::Vector2f &position) {
        sprite.setPosition(position);
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void updateAnimation() {
        AnimationManager::update(currentAnimation, sprite);
    }

    void draw(sf::RenderWindow &window) {
        AnimationManager::update(currentAnimation, sprite);
        window.draw(sprite);
    }

protected:
    sf::Sprite sprite;
    sf::Texture enemyHitTexture;
    sf::Texture enemyIdleTexture;
    sf::Texture enemyDeathTexture;
    sf::Texture enemyMoveDownTexture;
    sf::Texture enemyMoveRightTexture;
    sf::Texture enemyMoveLeftTexture;
    sf::Texture enemyMoveUpTexture;
    std::string currentAnimation;
    std::string enemyName;
    sf::Vector2f spawnPoint;
    sf::Vector2f direction;
    float speed;
    float tileSize = 32.0f;
    float detectionRange;

    void setTexture(const std::string &animationName, const sf::Vector2f &scale) {
        if (animationName == enemyName + "Hit") {
            sprite.setTexture(enemyHitTexture);
        } else if (animationName == enemyName + "Idle") {
            sprite.setTexture(enemyIdleTexture);
        } else if (animationName == enemyName + "Death") {
            sprite.setTexture(enemyDeathTexture);
        } else if (animationName == enemyName + "MoveDown") {
            sprite.setTexture(enemyMoveDownTexture);
        } else if (animationName == enemyName + "MoveRight") {
            sprite.setTexture(enemyMoveRightTexture);
            sprite.setScale(scale);
        } else if (animationName == enemyName + "MoveLeft") {
            sprite.setTexture(enemyMoveLeftTexture);
        } else if (animationName == enemyName + "MoveUp") {
            sprite.setTexture(enemyMoveUpTexture);
        }
    }

    static void setupAnimation(sf::Texture &texture, const std::string &animationName, const std::string &filePath,
                               sf::Vector2i frameCount, sf::Vector2i frameSize, sf::Vector2i startPosition,
                               int frequency) {
        if (!texture.loadFromFile(filePath)) {
            std::cerr << "Failed to load texture: " << filePath << "\n";
        } else {
            AnimationManager::addAnimation(animationName, texture, frameCount, frameSize, startPosition, frequency);
        }
    }
};

class BatEnemy : public enemy {
public:
    BatEnemy()
        : enemy("Bat", "assets/enemy/bat/BatFlyIdle.png", "assets/enemy/bat/BatAttack.png", "assets/enemy/bat/BatDie.png",
                "assets/enemy/bat/BatFlyIdle.png", "assets/enemy/bat/BatFlyIdle.png", "assets/enemy/bat/BatFlyIdle.png", "assets/enemy/bat/BatFlyIdle.png",
                0.6f, 100.f) {}
};
#endif