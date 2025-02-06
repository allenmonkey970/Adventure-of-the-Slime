// Created by antho on 2/5/2025.

#ifndef SLIME_H
#define SLIME_H

#include <SFML/Graphics.hpp>
#include <iostream>

class slime {
public:
    slime() : idle0FileName("assets/slime/Idle/0.png"), idle0(), idle0Sprite(idle0) {
        if (!idle0.loadFromFile(idle0FileName)) {
            std::cout << "Cannot load image idle0 \n";
        }
        // Define the texture rectangle using IntRect
        idle0Sprite.setTextureRect(sf::IntRect({30, 30}, {19, 15}));
        idle0Sprite.setScale({1,1});
    }

    void draw(sf::RenderWindow &window) {
        window.draw(idle0Sprite);
    }

private:
    sf::Texture idle0;
    sf::Sprite idle0Sprite;
    std::string idle0FileName;
};

#endif // SLIME_H
