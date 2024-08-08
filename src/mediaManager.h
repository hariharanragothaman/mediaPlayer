//
// Created by Hariharan Ragothaman on 8/7/24.
//

#ifndef PYCPPLINK_MEDIAMANAGER_H
#define PYCPPLINK_MEDIAMANAGER_H

#include "loader.h"
#include "player.h"
#include "queue.h"

class MediaManager
{
private:
    Loader loader_;
    Player player_;
    MediaQueue queue_;

public:
    MediaManager(): loader_(), player_() {}
    void addMediaToQueue(const std::string& file_path, const std:: string& type);
    void play();
    void stop();
    void playNext();
    void pause();
    void playMedia(const std::string& file_path, const std:: string& type);
};


#endif //PYCPPLINK_MEDIAMANAGER_H
