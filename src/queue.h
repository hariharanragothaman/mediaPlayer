#ifndef MEDIAPLAYER_QUEUE_H
#define MEDIAPLAYER_QUEUE_H

#include <deque>
#include "media.h"

class MediaQueue
{
private:
    std::deque<std::shared_ptr<Media>> queue_;
public:
    void addMedia(std::shared_ptr<Media> media);
    std::shared_ptr<Media> getNextMedia();
    std::shared_ptr<Media> peekNextMedia() const;
    void removeNextMedia();
    bool isEmpty() const;
};

#endif //MEDIAPLAYER_QUEUE_H
