#include <SFML/Graphics.hpp>
#include <iostream>

#include "mediaManager.h"

int main()
{

    /* Media Manager Object */
    MediaManager manager;
    manager.addMediaToQueue("/tmp/example.mp3", "audio");

    /* GUI Begins */

    sf::RenderWindow window(sf::VideoMode(500, 500), "Media Player");

    sf::Texture playTexture,stopTexture, pauseTexture, fastForwardTexture, rewindTexture, shuffleTexture, loadMusicTexture;

    if (!playTexture.loadFromFile("play.png") ||
        !stopTexture.loadFromFile("stop.png") ||
        !pauseTexture.loadFromFile("pause.png"))
    {
        std::cerr << "Failed to load button textures" << std::endl;
        return 1;
    }


    sf::Sprite playButton(playTexture);
    sf::Sprite stopButton(stopTexture);
    sf::Sprite pauseButton(pauseTexture);
    sf::Sprite fastForwardButton(fastForwardTexture);
    sf::Sprite rewindButton(rewindTexture);
    sf::Sprite shuffleButton(shuffleTexture);
    sf::Sprite loadMusicButton(loadMusicTexture);

    playButton.setPosition(100, 100);
    stopButton.setPosition(160, 100);
    pauseButton.setPosition(220, 100);
    fastForwardButton.setPosition(280, 100);
    rewindButton.setPosition(340, 100);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (playButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    std::cout << "Play Button Pressed" << std::endl;
                    manager.playNext();
                }
                if (stopButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    std::cout << "Stop Button Pressed" << std::endl;
                    manager.stopMedia();
                }
                if (pauseButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    std::cout << "Pause Button Pressed" << std::endl;
                    // Trigger pause
                }
                if (fastForwardButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    std::cout << "Fast Forward Button Pressed" << std::endl;
                    // Trigger fast forward
                }
                if (rewindButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    std::cout << "Rewind Button Pressed" << std::endl;
                    // Trigger rewind
                }
            }
        }

        window.clear(sf::Color::Transparent);
        window.draw(playButton);
        window.draw(stopButton);
        window.draw(pauseButton);
        window.draw(fastForwardButton);
        window.draw(rewindButton);
        window.display();
    }

    return 0;
}
