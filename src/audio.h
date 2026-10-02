#ifndef MEDIAPLAYER_AUDIO_H
#define MEDIAPLAYER_AUDIO_H

#include "media.h"
#include <SFML/Audio.hpp>

#include <iostream>

class Audio: public Media
{
private:
    sf::Music music;
    float cachedDuration{0.f};

public:
    explicit Audio(const std::string& file)
    {
        file_path = file;
        if(!music.openFromFile(file_path))
        {
            throw std::runtime_error("Failed to load music file: " + file_path);
        }
        cachedDuration = music.getDuration().asSeconds();
    }

    void displayInfo() const override
    {
        std::cout << "Audio file: " << file_path << std::endl;
    }

    void playMedia() override
    {
        std::cout << "Playing audio: " << file_path << std::endl;
        music.play();
    }

    void stopMedia() override
    {
        music.stop();
    }

    void pauseMedia() override
    {
        music.pause();
    }

    void resumeMedia() override
    {
        music.play();
    }

    float getDurationSeconds() const override
    {
        return cachedDuration;
    }

    float getPlayingOffsetSeconds() const override
    {
        return music.getPlayingOffset().asSeconds();
    }

    void seekTo(float seconds) override
    {
        music.setPlayingOffset(sf::seconds(seconds));
    }

    void setVolume(float volume) override
    {
        music.setVolume(volume);
    }

    float getVolume() const override
    {
        return music.getVolume();
    }

    bool isStopped() const override
    {
        return music.getStatus() == sf::Music::Stopped;
    }
};

#endif //MEDIAPLAYER_AUDIO_H
