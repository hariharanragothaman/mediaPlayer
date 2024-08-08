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

    std::atomic<bool> isPaused;
    std::atomic<bool> isPlaying;

    std::chrono::seconds currentPos;
    std::mutex mtx;
    std::condition_variable cv;

//    void playback()
//    {
//        if(current_media)
//        {
//            current_media->playMedia();
//            while(isPlaying)
//            {
//                std::this_thread::sleep_for(std::chrono::seconds(1));
//            }
//        }
//        std::cout << "Stopped.. Playing Track" << std::endl;
//    }

    void playback()
    {
        if (!current_media) return;

        std::cout << "Starting playback: ";
        current_media->displayInfo();

        while (isPlaying)
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]{ return !isPaused; });  // Only proceed if not paused

            // Simulate playback by sleeping and incrementing position
            current_media->playMedia();

            std::this_thread::sleep_for(std::chrono::seconds(1));
            currentPos += std::chrono::seconds(1);  // Increment the current position

            std::cout << "Playing... " << currentPos.count() << "s" << std::endl;

            if (!isPlaying)
                break;
        }

        isPlaying = false;
        std::cout << "Playback stopped." << std::endl;
    }


public:
    void play(std::shared_ptr<Media> media);
    void stop();
    void pause();
    bool hasFinishedPlaying() const;
};

#endif //PYCPPLINK_PLAYER_H
