#include <SFML/Graphics.hpp>
#include "slime.h"

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Adventure of Slime");
    window.setFramerateLimit(30);

    Slime mainSlime;
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        mainSlime.draw(window);
        window.display();
    }
}
