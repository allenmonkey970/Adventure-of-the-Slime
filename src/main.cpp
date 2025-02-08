#include <SFML/Graphics.hpp>
#include "slime.h"
#include "icon.h"

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Adventure of the Slime");
    window.setFramerateLimit(30);
    window.setVerticalSyncEnabled(true);
    Icon icon;
    icon.draw(window);


    Slime mainSlime;
    mainSlime.setScale("hit", {3, 3});


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
