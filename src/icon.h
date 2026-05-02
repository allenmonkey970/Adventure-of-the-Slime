#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Icon {
public:
    Icon() {
        if (!image.loadFromFile("assets/icon/icon.png"))
            std::cerr << "Failed to load icon\n";
    }

    void setIcon(sf::RenderWindow& window) const {
        if (image.getSize().x > 0 && image.getSize().y > 0)
            window.setIcon(image);
    }

private:
    sf::Image image;
};
