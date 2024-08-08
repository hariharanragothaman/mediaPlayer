//
// Created by Hariharan Ragothaman on 8/7/24.
//

#ifndef PYCPPLINK_QUEUE_H
#define PYCPPLINK_QUEUE_H

#include <deque>
#include "media.h"

class MediaQueue
{
private:
    std::deque<std::shared_ptr<Media>> queue_;
public:
    void addMedia(std::shared_ptr<Media> media);
    std::shared_ptr<Media> getNextMedia();
};

#endif //PYCPPLINK_QUEUE_H
