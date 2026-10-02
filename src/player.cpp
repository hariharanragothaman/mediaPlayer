#include "player.h"
#include <iostream>

Player::~Player()
{
    stop();
}

void Player::play(std::shared_ptr<Media> media)
{
    stop();
    current_media = std::move(media);
    isPlaying = true;
    isPaused = false;
    std::cout << "Starting playback: ";
    current_media->displayInfo();
    current_media->playMedia();
}

void Player::update()
{
    if (!isPlaying || isPaused || !current_media || !current_media->isStopped())
        return;

    std::cout << "Playback finished (ended)" << std::endl;
    isPlaying = false;
    isPaused = false;

    if (onTrackFinished)
        onTrackFinished();
}

void Player::stop()
{
    isPlaying = false;
    isPaused = false;

    if (current_media)
    {
        current_media->stopMedia();
    }

    current_media = nullptr;
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

void Player::seekTo(float seconds)
{
    if (current_media)
    {
        current_media->seekTo(seconds);
    }
}

void Player::setVolume(float volume)
{
    if (current_media)
    {
        current_media->setVolume(volume);
    }
}

float Player::getVolume() const
{
    if (current_media)
        return current_media->getVolume();
    return 100.f;
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

void Player::setOnTrackFinished(std::function<void()> callback)
{
    onTrackFinished = std::move(callback);
}
