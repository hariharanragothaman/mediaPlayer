#include "player.h"
#include <iostream>


void Player::play(std::shared_ptr<Media> media)
{
    std::cout << "Player Class:: Beginning to Play Media" << std::endl;
    current_media = media;
    current_media->playMedia();
}

void Player::stop()
{
    if (current_media)
    {
        std::cout << "Stopping: " << current_media << std::endl;
        current_media = nullptr;
    }
}
