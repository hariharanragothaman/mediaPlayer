#include "queue.h"
#include <iostream>

void MediaQueue::addMedia(std::shared_ptr<Media> media)
{
    tracks_.push_back(std::move(media));
    std::cout << "Playlist size: " << tracks_.size() << std::endl;
}

void MediaQueue::clear()
{
    tracks_.clear();
    currentIndex_ = -1;
}

std::shared_ptr<Media> MediaQueue::getNextMedia()
{
    if (tracks_.empty())
        return nullptr;

    currentIndex_++;
    if (currentIndex_ >= static_cast<int>(tracks_.size()))
    {
        currentIndex_ = 0;
    }
    return tracks_[currentIndex_];
}

std::shared_ptr<Media> MediaQueue::getPreviousMedia()
{
    if (tracks_.empty())
        return nullptr;

    currentIndex_--;
    if (currentIndex_ < 0)
    {
        currentIndex_ = static_cast<int>(tracks_.size()) - 1;
    }
    return tracks_[currentIndex_];
}

std::shared_ptr<Media> MediaQueue::getCurrentMedia() const
{
    if (tracks_.empty() || currentIndex_ < 0 || currentIndex_ >= static_cast<int>(tracks_.size()))
        return nullptr;
    return tracks_[currentIndex_];
}

std::shared_ptr<Media> MediaQueue::getMediaAt(int index)
{
    if (index < 0 || index >= static_cast<int>(tracks_.size()))
        return nullptr;
    currentIndex_ = index;
    return tracks_[currentIndex_];
}

std::shared_ptr<Media> MediaQueue::peekNextMedia() const
{
    if (tracks_.empty())
        return nullptr;
    int next = currentIndex_ + 1;
    if (next >= static_cast<int>(tracks_.size()))
        next = 0;
    return tracks_[next];
}

bool MediaQueue::isEmpty() const
{
    return tracks_.empty();
}

int MediaQueue::size() const
{
    return static_cast<int>(tracks_.size());
}

int MediaQueue::getCurrentIndex() const
{
    return currentIndex_;
}
