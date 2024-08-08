//
// Created by Hariharan Ragothaman on 8/7/24.
//

#ifndef PYCPPLINK_AUDIO_H
#define PYCPPLINK_AUDIO_H

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

    void play();

    void pause() override;
    void stop() override;

    void playMedia() override
    {
        std::cout << "Playing - Audio file: " << file_path << std::endl;
        music.play();
        while(music.getStatus() == sf::Music::Playing)
        {
            sf::sleep(sf::seconds(0.1));
        }
    }
};

#endif //PYCPPLINK_AUDIO_H
