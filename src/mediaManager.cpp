#include "mediaManager.h"
#include <iostream>
#include <filesystem>
#include <algorithm>

static const std::vector<std::string> SUPPORTED_EXTENSIONS = {
    ".mp3", ".ogg", ".wav", ".flac"
};

static bool isSupportedAudio(const std::filesystem::path& path)
{
    auto ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    for (const auto& supported : SUPPORTED_EXTENSIONS)
    {
        if (ext == supported)
            return true;
    }
    return false;
}

MediaManager::MediaManager(): loader_(), player_()
{
    player_.setOnTrackFinished([this]() {
        autoAdvancePending_ = true;
    });
}

void MediaManager::addMediaToQueue(const std::string& file_path, const std::string& type)
{
    try
    {
        auto media = loader_.loadMedia(file_path, type);
        if (media)
        {
            queue_.addMedia(std::move(media));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to load: " << file_path << " — " << e.what() << std::endl;
    }
}

void MediaManager::loadDirectory(const std::string& directory)
{
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory))
    {
        std::cerr << "Invalid directory: " << directory << std::endl;
        return;
    }

    stopMedia();
    queue_.clear();

    std::vector<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file() && isSupportedAudio(entry.path()))
        {
            files.push_back(entry.path());
        }
    }

    std::sort(files.begin(), files.end());

    for (const auto& f : files)
    {
        addMediaToQueue(f.string(), "audio");
    }

    std::cout << "Loaded " << queue_.size() << " tracks from " << directory << std::endl;
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
        std::cout << "Playlist is empty — add files to play" << std::endl;
    }
}

void MediaManager::playPrevious()
{
    auto media = queue_.getPreviousMedia();
    if (media)
    {
        player_.play(media);
    }
}

void MediaManager::playTrackAt(int index)
{
    auto media = queue_.getMediaAt(index);
    if (media)
    {
        player_.play(media);
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

void MediaManager::togglePlayPause()
{
    if (isPaused())
    {
        resumeMedia();
    }
    else if (isPlaying())
    {
        pauseMedia();
    }
    else
    {
        playNext();
    }
}

void MediaManager::seekTo(float seconds)
{
    player_.seekTo(seconds);
}

void MediaManager::setVolume(float volume)
{
    player_.setVolume(volume);
}

float MediaManager::getVolume() const
{
    return player_.getVolume();
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
    try
    {
        auto media = loader_.loadMedia(file_path, type);
        if (media)
        {
            player_.play(std::shared_ptr<Media>(std::move(media)));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to play: " << file_path << " — " << e.what() << std::endl;
    }
}

float MediaManager::getCurrentTimeSeconds() const
{
    auto media = player_.getCurrentMedia();
    if (!media)
        return 0.f;
    return media->getPlayingOffsetSeconds();
}

float MediaManager::getTotalTimeSeconds() const
{
    auto media = player_.getCurrentMedia();
    if (!media)
        return 0.f;
    return media->getDurationSeconds();
}

std::string MediaManager::getCurrentTrackName() const
{
    auto media = player_.getCurrentMedia();
    if (!media)
        return "";
    return media->getFileName();
}

int MediaManager::getCurrentTrackIndex() const
{
    return queue_.getCurrentIndex();
}

int MediaManager::getTrackCount() const
{
    return queue_.size();
}

const std::vector<std::shared_ptr<Media>>& MediaManager::getTracks() const
{
    return queue_.getTracks();
}

void MediaManager::checkAutoAdvance()
{
    if (autoAdvancePending_)
    {
        autoAdvancePending_ = false;
        playNext();
    }
}
