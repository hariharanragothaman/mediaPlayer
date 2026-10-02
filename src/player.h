#ifndef MEDIAPLAYER_PLAYER_H
#define MEDIAPLAYER_PLAYER_H

#include "media.h"

#include <functional>

class Player
{
private:
    std::shared_ptr<Media> current_media;
    bool isPlaying{false};
    bool isPaused{false};
    std::function<void()> onTrackFinished;

public:
    ~Player();

    void play(std::shared_ptr<Media> media);
    void update();
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
