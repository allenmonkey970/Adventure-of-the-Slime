//
// Created by antho on 2/5/2025.
//

#ifndef SLIME_H
#define SLIME_H
#include <SFML/Graphics.hpp>
#include <iostream>

class slime {
public:
    void idle() {
        if (!idle0.loadFromFile(idle0FileName, false, sf::IntRect({10, 10}, {19, 15}))) {
            std::cout << "Cannot load image idle0 \n";
        }
        sf::Sprite idle0Sprite(idle0);
        idle0Sprite.setTexture(idle0); // Set initial texture
        //Define the texture rectangle using FloatRect and convert to IntRect
        sf::FloatRect floatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(19.f, 15.f));
        idle0Sprite.setTextureRect(sf::IntRect(floatRect));
    }

    void draw(sf::RenderWindow &window) {
        window.draw(idle0Sprite);
    }

private:
    sf::Texture idle0;
    std::string idle0FileName = "assets/slime/Idle/0.png";
};


#endif //SLIME_H
