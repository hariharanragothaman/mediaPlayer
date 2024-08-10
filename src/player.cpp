#include "player.h"
#include <iostream>


void Player::play(std::shared_ptr<Media> media)
{
    std::cout << "Player Class:: Beginning to Play Media" << std::endl;
    if(mediaThread.joinable())
    {
        std::cout << "Joining the threads..." << std::endl;
        mediaThread.join();
    }
    current_media = media;
    mediaThread = std::thread(&Player::playback, this);
    isPlaying = true;
}

void Player::stop()
{
    if (mediaThread.joinable())
    {
        std::cout << "Stopping: " << current_media << std::endl;
        current_media->stopMedia();
        mediaThread.join();
        current_media = nullptr;
    }
}

void Player::pause()
{
    std::cout << "Player Class:: Pausing Media" << std::endl;
    if (mediaThread.joinable())
    {
        std::cout << "Pausing: " << current_media << std::endl;
        current_media->pauseMedia();
        mediaThread.join();
    }
}

bool Player::hasFinishedPlaying() const
{
    return !isPlaying;
}
