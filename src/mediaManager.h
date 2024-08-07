//
// Created by Hariharan Ragothaman on 8/7/24.
//

#ifndef PYCPPLINK_MEDIAMANAGER_H
#define PYCPPLINK_MEDIAMANAGER_H

#include "loader.h"
#include "player.h"

class MediaManager
{
private:
    Loader loader_;
    Player player_;
public:
    MediaManager(): loader_(), player_() {}
    void addMediaToQueue(const std::string& file_path, const std:: string& type);
    void playNext();
    void playMedia(const std::string& file_path, const std:: string& type);
};


#endif //PYCPPLINK_MEDIAMANAGER_H
