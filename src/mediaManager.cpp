#include "mediaManager.h"
#include "audio.h"
#include <iostream>

void MediaManager::addMediaToQueue(const std::string& file_path, const std::string& type)
{
    auto media = loader_.loadMedia(file_path, type);
    if (media)
    {
        queue_.addMedia(std::move(media));
    }
}

void MediaManager::playNext()
{
    auto media = queue_.getNextMedia();
    if (media)
    {
        player_.play(media);
    }
    else
    {
        std::cout << "Queue is empty -- add files to play" << std::endl;
    }
}

void MediaManager::stopMedia()
{
    player_.stop();
}

void MediaManager::pauseMedia()
{
    player_.pause();
}

void MediaManager::resumeMedia()
{
    player_.resume();
}

bool MediaManager::isPaused() const
{
    return player_.getIsPaused();
}

bool MediaManager::isPlaying() const
{
    return !player_.hasFinishedPlaying();
}

void MediaManager::playMedia(const std::string& file_path, const std::string& type)
{
    auto media = loader_.loadMedia(file_path, type);
    if (media)
    {
        player_.play(std::shared_ptr<Media>(std::move(media)));
    }
}

sf::Time MediaManager::getCurrentTime()
{
    auto media = player_.getCurrentMedia();
    if (!media)
        return sf::Time::Zero;
    auto audio = std::dynamic_pointer_cast<Audio>(media);
    if (!audio)
        return sf::Time::Zero;
    return audio->getCurrentTime();
}

sf::Time MediaManager::getTotalTime()
{
    auto media = player_.getCurrentMedia();
    if (!media)
        return sf::Time::Zero;
    auto audio = std::dynamic_pointer_cast<Audio>(media);
    if (!audio)
        return sf::Time::Zero;
    return audio->getTotalTime();
}
