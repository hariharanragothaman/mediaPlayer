//
// Created by Hariharan Ragothaman on 8/7/24.
//

#ifndef PYCPPLINK_PLAYER_H
#define PYCPPLINK_PLAYER_H

#include "media.h"

#include <iostream>
#include <thread>

class Player
{
private:
    std::shared_ptr<Media> current_media;
    std::thread mediaThread;
    std::atomic<bool> isPlaying;
    void playback()
    {
        if(current_media)
        {
            current_media->playMedia();
            while(isPlaying)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        std::cout << "Stopped.. Playing Track" << std::endl;
    }

public:
    void play(std::shared_ptr<Media> media);
    void stop();
};

#endif //PYCPPLINK_PLAYER_H
