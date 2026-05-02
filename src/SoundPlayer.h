#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

class SoundPlayer {
public:
    SoundPlayer();
    void playSound(const std::string& filename);

private:
    sf::SoundBuffer buffer;
    sf::Sound sound;
};

#endif // SOUNDPLAYER_H
