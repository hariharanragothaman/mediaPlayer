//
// Created by Hariharan Ragothaman on 8/7/24.
//

#ifndef PYCPPLINK_MEDIAMANAGER_H
#define PYCPPLINK_MEDIAMANAGER_H

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
    void addMediaToQueue(const std::string& file_path, const std:: string& type);
    void playNext();
    void playMedia(const std::string& file_path, const std:: string& type);
    void stopMedia();
    sf::Time getCurrentTime();
    sf::Time getTotalTime();
};


#endif //PYCPPLINK_MEDIAMANAGER_H
