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
    auto media = queue_.peekNextMedia();
    if (media)
    {
        player_.play(std::move(media));
    }
    else
    {
        std::cout << "Queue is Empty - Please add a file to play" << std::endl;
    }
}

void MediaManager::stopMedia()
{
    player_.stop();
    std::cout << "is Queue Empty: " << queue_.isEmpty() << std::endl;
}

void MediaManager::playMedia(const std::string& file_path, const std::string& type)
{
    auto media = loader_.loadMedia(file_path, type);
    media->playMedia();
}

sf::Time MediaManager::getCurrentTime()
{
    auto media = queue_.peekNextMedia();
    std::shared_ptr<Audio> audio = std::dynamic_pointer_cast<Audio>(media);
    auto time = audio->getCurrentTime();
    return time;
}

sf::Time MediaManager::getTotalTime()
{
    auto media = queue_.peekNextMedia();
    std::shared_ptr<Audio> audio = std::dynamic_pointer_cast<Audio>(media);
    auto time = audio->getTotalTime();
    return time;
}
