#ifndef ICON_H
#define ICON_H

#include <SFML/Graphics.hpp>
#include <iostream>

// Class to manage the icon for the exe
class Icon {
public:
    // Constructor to load the icon
    Icon() {
        // Load the image from the pathway
        if (!image.loadFromFile(imageFile)) {
            std::cerr << "Failed loading icon" << std::endl;
        }
    }

    // Method to set the window icon
    void draw(sf::RenderWindow& window) const {
        // Check if the image has been loaded before setting the icon
        if (image.getSize().x > 0 && image.getSize().y > 0) {
            window.setIcon(image);
        }
    }

private:
    // Image to hold the icon
    sf::Image image;
    std::string imageFile = "assets/icon/icon.png";
};

#endif // ICON_H
