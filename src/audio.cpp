#include "audio.h"

#include <SFML/Audio.hpp>
#include <iostream>

void Audio::play()
{
    std::cout << "Playing - Audio file: " << file_path << std::endl;
    music.play();
    while(music.getStatus() == sf::Music::Playing)
    {
        sf::sleep(sf::seconds(0.1));
    }
}

void Audio::pause() 
{
    music.pause();
}

void Audio::stop()
{
    music.stop();
}
