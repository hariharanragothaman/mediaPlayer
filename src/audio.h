#ifndef MEDIAPLAYER_AUDIO_H
#define MEDIAPLAYER_AUDIO_H

#include "media.h"
#include <SFML/Audio.hpp>

#include <iostream>

class Audio: public Media
{
private:
    sf::Music music;
public:
    explicit Audio(const std::string& file)
    {
        file_path = file;
        if(!music.openFromFile(file_path))
        {
            throw std::runtime_error("Failed to load music file: " + file_path);
        }
    }

    void displayInfo() const override
    {
        std::cout << "Audio file: " << file_path << std::endl;
    }

    void playMedia() override
    {
        std::cout << "Playing audio: " << file_path << std::endl;
        music.play();
        while(music.getStatus() != sf::Music::Stopped)
        {
            sf::sleep(sf::seconds(0.1));
        }
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

    sf::Time getCurrentTime();
    sf::Time getTotalTime();
};

#endif //MEDIAPLAYER_AUDIO_H
