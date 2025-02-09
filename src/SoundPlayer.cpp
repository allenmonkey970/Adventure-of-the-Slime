#include "SoundPlayer.h"

SoundPlayer::SoundPlayer() : buffer(), sound(buffer) {
    // Constructor with explicit member initialization
}

void SoundPlayer::playSound(const std::string& filename) {
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "Failed to load sound file: " << filename << std::endl;
        return;
    }
    sound.setBuffer(buffer);
    sound.setVolume(10.f); // Set the volume to 10%
    sound.play();
    // Wait until the sound finishes playing
    while (sound.getStatus() == sf::Sound::Status::Playing) {
        sf::sleep(sf::milliseconds(100)); // Sleep for a short duration
    }
}
