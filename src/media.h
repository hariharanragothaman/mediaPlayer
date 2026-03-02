#ifndef MEDIA_H
#define MEDIA_H

#include <string>

class Media
{
protected:
    std::string file_path;
public:
    explicit Media(const std::string& file_path);
    Media();

    virtual ~Media() = default;

    virtual void displayInfo() const = 0;
    virtual void playMedia() = 0;
    virtual void stopMedia() = 0;
    virtual void pauseMedia() = 0;
    virtual void resumeMedia() = 0;
};

#endif // MEDIA_H
