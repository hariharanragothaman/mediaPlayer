#include <queue>

class Queue {
private:
    std::deque<std::shared_ptr<Media>> queue_;
public:
    void addMedia(std::shared_ptr<Media> media) {
        queue_.push_back(std::move(media)); // Use std::move to pass ownership
    }

    std::shared_ptr<Media> getNextMedia() {
        if (queue_.empty()) return nullptr;
        auto media = std::move(queue_.front()); // Use std::move to efficiently remove the media from the queue
        queue_.pop_front();
        return media;
    }
};
