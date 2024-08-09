#include "queue.h"
#include <iostream>


void MediaQueue::addMedia(std::shared_ptr<Media> media)
{
    queue_.push_back(std::move(media));
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

std::shared_ptr<Media> MediaQueue::peekNextMedia()
{
    if (queue_.empty())
        return nullptr;
    return queue_.front();  // Just peek, don't pop
}

void MediaQueue::removeNextMedia()
{
    if (!queue_.empty())
        queue_.pop_front();  // Now we remove the media from the queue
}

bool MediaQueue::isEmpty()
{
    return queue_.empty();
}
