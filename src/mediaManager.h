#ifndef MEDIAPLAYER_MEDIAMANAGER_H
#define MEDIAPLAYER_MEDIAMANAGER_H

#include "loader.h"
#include "player.h"
#include "queue.h"

#include <SFML/Graphics.hpp>

class MediaManager
{
private:
    Loader loader_;
    Player player_;
    MediaQueue queue_;

public:
    MediaManager(): loader_(), player_() {}
    void addMediaToQueue(const std::string& file_path, const std::string& type);
    void playNext();
    void playMedia(const std::string& file_path, const std::string& type);
    void stopMedia();
    void pauseMedia();
    void resumeMedia();
    bool isPaused() const;
    bool isPlaying() const;
    sf::Time getCurrentTime();
    sf::Time getTotalTime();
};

#endif //MEDIAPLAYER_MEDIAMANAGER_H
