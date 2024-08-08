#include "queue.h"
#include <deque>
#include <iostream>


void MediaQueue::addMedia(std::shared_ptr<Media> media)
{
    queue_.push_back(std::move(media)); // Use std::move to pass ownership
    std::cout << "The queue size is: " << queue_.size() << std::endl;
}

std::shared_ptr<Media> MediaQueue::getNextMedia()
{
    if (queue_.empty())
        return nullptr;
    auto media = std::move(queue_.front()); // Use std::move to efficiently remove the media from the queue
    queue_.pop_front();
    return media;
}
