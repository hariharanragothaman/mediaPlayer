#ifndef MEDIAPLAYER_PLAYER_H
#define MEDIAPLAYER_PLAYER_H

#include "media.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <functional>

class Player
{
private:
    std::shared_ptr<Media> current_media;
    std::thread mediaThread;
    std::atomic<bool> isPlaying{false};
    std::atomic<bool> isPaused{false};
    std::function<void()> onTrackFinished;

    void playback()
    {
        if (!current_media)
        {
            std::cout << "Current Media is not set -- exiting" << std::endl;
            return;
        }

        std::cout << "Starting playback: ";
        current_media->displayInfo();
        current_media->playMedia();

        std::cout << "Playback finished" << std::endl;
        isPlaying = false;
        isPaused = false;

        if (onTrackFinished)
            onTrackFinished();
    }

public:
    ~Player();

    void play(std::shared_ptr<Media> media);
    void stop();
    void pause();
    void resume();
    void seekTo(float seconds);
    void setVolume(float volume);
    float getVolume() const;
    bool hasFinishedPlaying() const;
    bool getIsPaused() const;
    std::shared_ptr<Media> getCurrentMedia() const;

    void setOnTrackFinished(std::function<void()> callback);
};

#endif //MEDIAPLAYER_PLAYER_H
