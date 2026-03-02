#include "player.h"
#include <iostream>

void Player::play(std::shared_ptr<Media> media)
{
    stop();
    current_media = media;
    isPlaying = true;
    isPaused = false;
    mediaThread = std::thread(&Player::playback, this);
}

void Player::stop()
{
    if (current_media)
    {
        current_media->stopMedia();
    }
    if (mediaThread.joinable())
    {
        mediaThread.join();
    }
    current_media = nullptr;
    isPlaying = false;
    isPaused = false;
}

void Player::pause()
{
    if (current_media && isPlaying && !isPaused)
    {
        current_media->pauseMedia();
        isPaused = true;
    }
}

void Player::resume()
{
    if (current_media && isPaused)
    {
        current_media->resumeMedia();
        isPaused = false;
    }
}

bool Player::hasFinishedPlaying() const
{
    return !isPlaying;
}

bool Player::getIsPaused() const
{
    return isPaused;
}

std::shared_ptr<Media> Player::getCurrentMedia() const
{
    return current_media;
}
