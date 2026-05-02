#include "SoundPlayer.h"

SoundPlayer::SoundPlayer() : buffer(), sound(buffer) {}

void SoundPlayer::playSound(const std::string& filename) {
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "Failed to load sound file: " << filename << "\n";
        return;
    }
    sound.setBuffer(buffer);
    sound.setVolume(10.f);
    sound.play();
}
