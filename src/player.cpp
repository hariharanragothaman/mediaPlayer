#include "player.h"
#include <iostream>


void Player::play(std::shared_ptr<Media> media)
{
    std::cout << "Player Class:: Beginning to Play Media" << std::endl;
    if(mediaThread.joinable())
    {
        mediaThread.join();
    }
    current_media = media;
    isPlaying = true;
    isPaused = false;
    mediaThread = std::thread(&Player::playback, this);
}

void Player::stop()
{
    isPlaying = false;
    if (mediaThread.joinable())
    {
        std::cout << "Stopping: " << current_media << std::endl;
        current_media->stopMedia();
        mediaThread.join();
        //current_media = nullptr;
    }
}

void Player::pause()
{
    std::lock_guard<std::mutex> lock(mtx);
    isPaused = true;
}

bool Player::hasFinishedPlaying() const
{
    return !isPlaying;
}
