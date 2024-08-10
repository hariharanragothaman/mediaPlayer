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
//    std::atomic<bool> isPlaying;
    bool isPlaying;
    std::chrono::seconds currentPos;
    std::mutex mtx;
    std::condition_variable cv;

    void playback()
    {
        if (!current_media)
        {
            std::cout << "Current Media is not set.. - Hence exiting" << std::endl;
            return;
        }

        std::cout << "Starting playback: ";
        current_media->displayInfo();
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{return isPlaying;});
        current_media->playMedia();

        std::cout << "Finished Playing the track ..." << std::endl;
        lock.unlock();
        isPlaying = false;
        // After finshed playing - we should remove from the queue? right?
    }


public:
    void play(std::shared_ptr<Media> media);
    void stop();
    void pause();
    bool hasFinishedPlaying() const;
};

#endif //PYCPPLINK_PLAYER_H
