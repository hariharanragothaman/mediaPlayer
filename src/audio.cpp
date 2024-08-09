#include <SFML/Graphics.hpp>
#include "audio.h"

sf::Time Audio::getCurrentTime()
{
    return music.getPlayingOffset();
}

sf::Time Audio::getTotalTime()
{
    return music.getDuration();
}
