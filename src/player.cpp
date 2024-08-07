#include "player.h"
#include <iostream>


void Player::play(std::shared_ptr<Media> media)
{
    current_media = media;
    current_media->displayInfo();
}

void Player::stop()
{
    if (current_media)
    {
        std::cout << "Stopping: " << current_media << std::endl;
        current_media = nullptr;
    }
}
