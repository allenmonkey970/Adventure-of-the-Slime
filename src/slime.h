// Created by antho on 2/5/2025.

#ifndef SLIME_H
#define SLIME_H

#include <SFML/Graphics.hpp>
#include <iostream>

class slime {
public:
    void idle() {
        if (!idle0.loadFromFile(idle0FileName)) {
            std::cout << "Cannot load image idle0 \n";
        }
        idle0Sprite.setTexture(idle0);
        // Define the texture rectangle using IntRect
        idle0Sprite.setTextureRect(sf::IntRect({10, 10}, {19, 15}));
    }

    void draw(sf::RenderWindow &window) {
        window.draw(idle0Sprite);
    }

private:
    sf::Texture idle0;
    sf::Sprite idle0Sprite;
    std::string idle0FileName = "assets/slime/Idle/0.png";
};

#endif //SLIME_H
