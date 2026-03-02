#include "queue.h"
#include <iostream>

void MediaQueue::addMedia(std::shared_ptr<Media> media)
{
    queue_.push_back(std::move(media));
    std::cout << "Queue size: " << queue_.size() << std::endl;
}

std::shared_ptr<Media> MediaQueue::getNextMedia()
{
    if (queue_.empty())
        return nullptr;
    auto media = std::move(queue_.front());
    queue_.pop_front();
    return media;
}

std::shared_ptr<Media> MediaQueue::peekNextMedia() const
{
    if (queue_.empty())
        return nullptr;
    return queue_.front();
}

void MediaQueue::removeNextMedia()
{
    if (!queue_.empty())
        queue_.pop_front();
}

bool MediaQueue::isEmpty() const
{
    return queue_.empty();
}
