#include "mediaManager.h"

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
}

void MediaManager::playMedia(const std::string& file_path, const std::string& type)
{
    auto media = loader_.loadMedia(file_path, type);
    media->playMedia();
}
