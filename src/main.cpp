#include <SFML/Graphics.hpp>
#include "slime.h"

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Adventure of Slime");
    window.setFramerateLimit(60);


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}
