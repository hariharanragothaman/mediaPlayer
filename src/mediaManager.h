#ifndef MEDIAPLAYER_MEDIAMANAGER_H
#define MEDIAPLAYER_MEDIAMANAGER_H

#include "loader.h"
#include "player.h"
#include "queue.h"

#include <string>
#include <vector>

class MediaManager
{
private:
    Loader loader_;
    Player player_;
    MediaQueue queue_;
    bool autoAdvancePending_{false};

public:
    MediaManager();

    void addMediaToQueue(const std::string& file_path, const std::string& type);
    void loadDirectory(const std::string& directory);
    void playNext();
    void playPrevious();
    void playTrackAt(int index);
    void playMedia(const std::string& file_path, const std::string& type);
    void stopMedia();
    void pauseMedia();
    void resumeMedia();
    void togglePlayPause();

    void seekTo(float seconds);
    void setVolume(float volume);
    float getVolume() const;

    bool isPaused() const;
    bool isPlaying() const;

    float getCurrentTimeSeconds() const;
    float getTotalTimeSeconds() const;

    std::string getCurrentTrackName() const;
    int getCurrentTrackIndex() const;
    int getTrackCount() const;
    const std::vector<std::shared_ptr<Media>>& getTracks() const;

    void checkAutoAdvance();
};

#endif //MEDIAPLAYER_MEDIAMANAGER_H
