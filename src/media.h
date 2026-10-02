#ifndef MEDIA_H
#define MEDIA_H

#include <string>
#include <filesystem>

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

    virtual float getDurationSeconds() const = 0;
    virtual float getPlayingOffsetSeconds() const = 0;
    virtual void seekTo(float seconds) = 0;
    virtual void setVolume(float volume) = 0;
    virtual float getVolume() const = 0;
    virtual bool isStopped() const = 0;

    std::string getFilePath() const { return file_path; }
    std::string getFileName() const
    {
        return std::filesystem::path(file_path).stem().string();
    }
};

#endif // MEDIA_H
