#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "mediaManager.h"

int main()
{
    /* MediaManger Object */
    MediaManager manager;
    manager.addMediaToQueue("/tmp/example.mp3", "audio");

    /* Setting the bars for visualization */
    const int numBars = 64;
    std::vector<sf::RectangleShape> bars(numBars);

    for (int i = 0; i < numBars; ++i) {
        bars[i].setSize(sf::Vector2f(6, 1));  // Initial size (width, height)
        bars[i].setFillColor(sf::Color::Magenta);
        bars[i].setPosition(i * 7 + 25, 450);  // Position bars slightly lower (450 is lower in a 500px height window)
        bars[i].setOrigin(0, 0);  // Set origin to the bottom-left corner of the bar
    }


    /* Setting the Status Track bar */
    sf::RectangleShape statusBar(sf::Vector2f(15, 20));  // Initial width 0, height 10
    statusBar.setFillColor(sf::Color::Green);
    statusBar.setPosition(50, 200);  // Position it near the bottom of the window

    /* Setting a background for the status bar */
    // Create a background rectangle for the status bar
    sf::RectangleShape statusBarBackground(sf::Vector2f(400, 10));  // Background size matches the maximum width of the status bar
    statusBarBackground.setFillColor(sf::Color::Yellow);  // Fixed yellow background
    statusBarBackground.setPosition(50, 200);  // Same position as the status bar



    // Load a font for displaying the current time
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    // Create a text object for the current time
    sf::Text currentTimeText;
    currentTimeText.setFont(font);
    currentTimeText.setCharacterSize(12);  // Size of the text
    currentTimeText.setFillColor(sf::Color::White);  // Text color
    currentTimeText.setPosition(50, 180);  // Position it above the status bar

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(500, 500), "Media Player");

    sf::Texture playTexture, stopTexture, pauseTexture, loadMusicTexture;
    if (!playTexture.loadFromFile("play.png") ||
        !stopTexture.loadFromFile("stop.png") ||
        !pauseTexture.loadFromFile("pause.png") ||
        !loadMusicTexture.loadFromFile("loadMusic.png")
        )
    {
        std::cerr << "Failed to load button textures" << std::endl;
        return 1;
    }

    sf::Sprite playButton(playTexture);
    sf::Sprite stopButton(stopTexture);
    sf::Sprite pauseButton(pauseTexture);
    sf::Sprite loadMusicButton(loadMusicTexture);

    playButton.setPosition(100, 100);
    stopButton.setPosition(160, 100);
    pauseButton.setPosition(220, 100);
    loadMusicButton.setPosition(280, 100);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
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
                    manager.pauseMedia();
                }
                if (loadMusicButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    std::cout << "Load Music Button Pressed" << std::endl;

                }
            }
        }

        // Move the update logic outside the event loop to ensure continuous updates
        if (clock.getElapsedTime().asMilliseconds() > 100)
        {  // Update every 100ms
            for (int i = 0; i < numBars; ++i)
            {
                float amplitude = static_cast<float>(std::rand()) / RAND_MAX;
                bars[i].setSize(sf::Vector2f(6, amplitude * 150));  // Adjust height (150 is the maximum height of bars)
                bars[i].setPosition(bars[i].getPosition().x, 450 - bars[i].getSize().y);  // Adjust position to grow upwards
            }
            clock.restart();
        }

        // Update track status bar
        sf::Time currentTime = manager.getCurrentTime();  // Assume this function gives the current playback time
        sf::Time totalTime = manager.getTotalTime();      // Assume this function gives the total duration of the track

        if (totalTime != sf::Time::Zero)
        {
            float progress = currentTime.asSeconds() / totalTime.asSeconds();

            statusBar.setSize(sf::Vector2f(400 * progress, 10));  // Update the width of the status bar based on progress
//            std::cout << "Current Time: " << currentTime.asSeconds() << "s, "
//                      << "Total Time: " << totalTime.asSeconds() << "s, "
//                      << "Progress: " << progress * 100 << "%" << std::endl;


            // Update the current time text
            int minutes = static_cast<int>(currentTime.asSeconds()) / 60;
            int seconds = static_cast<int>(currentTime.asSeconds()) % 60;

            std::stringstream timeStream;
            timeStream << std::setw(2) << std::setfill('0') << minutes << ":"
                       << std::setw(2) << std::setfill('0') << seconds;
            currentTimeText.setString(timeStream.str());

        }

        window.clear(sf::Color::Black);  // Use a solid background color

        for (const auto& bar : bars) {
            window.draw(bar);  // Draw each bar
        }

        window.draw(playButton);
        window.draw(stopButton);
        window.draw(pauseButton);
        window.draw(loadMusicButton);

        window.draw(statusBarBackground);
        window.draw(statusBar);  // Draw status bar first
        window.draw(currentTimeText);

        window.display();
    }

    return 0;
}
