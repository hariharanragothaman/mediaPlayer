#ifndef MEDIAPLAYER_QUEUE_H
#define MEDIAPLAYER_QUEUE_H

#include <vector>
#include <string>
#include "media.h"

class MediaQueue
{
private:
    std::vector<std::shared_ptr<Media>> tracks_;
    int currentIndex_{-1};

public:
    void addMedia(std::shared_ptr<Media> media);
    void clear();

    std::shared_ptr<Media> getNextMedia();
    std::shared_ptr<Media> getPreviousMedia();
    std::shared_ptr<Media> getCurrentMedia() const;
    std::shared_ptr<Media> getMediaAt(int index);

    std::shared_ptr<Media> peekNextMedia() const;
    bool isEmpty() const;
    int size() const;
    int getCurrentIndex() const;

    const std::vector<std::shared_ptr<Media>>& getTracks() const { return tracks_; }
};

#endif //MEDIAPLAYER_QUEUE_H
