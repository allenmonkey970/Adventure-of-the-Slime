//
// Created by antho on 2/5/2025.
//

#ifndef SLIME_H
#define SLIME_H
#include <SFML/Graphics.hpp>
#include <iostream>

class slime {
public:
    void idle(){
        if (!idle0.loadFromFile(idle0FileName, false, sf::IntRect({10, 10}, {19, 15})))
            {
            std::cout<<"Cannot load image idle0 \n";
            }
        sf::Sprite SpriteIdle0(idle0);
    }
    void draw(sf::RenderWindow &window) {
        window.draw(SpriteIdle0);
    }
private:
    sf::Texture idle0;
    std::string idle0FileName = "assets/slime/Idle/0.png";


};



#endif //SLIME_H
