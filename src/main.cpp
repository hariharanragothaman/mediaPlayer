#include <SFML/Graphics.hpp>
#include <iostream>

#include "mediaManager.h"

#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    MediaManager manager;
    manager.addMediaToQueue("/tmp/example.mp3", "audio");

    const int numBars = 64;
    std::vector<sf::RectangleShape> bars(numBars);

    for (int i = 0; i < numBars; ++i) {
        bars[i].setSize(sf::Vector2f(6, 1));  // Initial size (width, height)
        bars[i].setFillColor(sf::Color::Magenta);
        bars[i].setPosition(i * 7 + 25, 450);  // Position bars slightly lower (450 is lower in a 500px height window)
        bars[i].setOrigin(0, 0);  // Set origin to the bottom-left corner of the bar
    }

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(500, 500), "Media Player");

    sf::Texture playTexture, stopTexture, pauseTexture;
    if (!playTexture.loadFromFile("play.png") ||
        !stopTexture.loadFromFile("stop.png") ||
        !pauseTexture.loadFromFile("pause.png")) {
        std::cerr << "Failed to load button textures" << std::endl;
        return 1;
    }

    sf::Sprite playButton(playTexture);
    sf::Sprite stopButton(stopTexture);
    sf::Sprite pauseButton(pauseTexture);

    playButton.setPosition(100, 100);
    stopButton.setPosition(160, 100);
    pauseButton.setPosition(220, 100);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (playButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    std::cout << "Play Button Pressed" << std::endl;
                    manager.playNext();
                }
                if (stopButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    std::cout << "Stop Button Pressed" << std::endl;
                    manager.stopMedia();
                }
                if (pauseButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    std::cout << "Pause Button Pressed" << std::endl;
                    // Trigger pause
                }
            }
        }

        // Move the update logic outside the event loop to ensure continuous updates
        if (clock.getElapsedTime().asMilliseconds() > 100) {  // Update every 100ms
            for (int i = 0; i < numBars; ++i) {
                float amplitude = static_cast<float>(std::rand()) / RAND_MAX;
                bars[i].setSize(sf::Vector2f(6, amplitude * 150));  // Adjust height (150 is the maximum height of bars)
                bars[i].setPosition(bars[i].getPosition().x, 450 - bars[i].getSize().y);  // Adjust position to grow upwards
            }
            clock.restart();
        }

        window.clear(sf::Color::Black);  // Use a solid background color

        for (const auto& bar : bars) {
            window.draw(bar);  // Draw each bar
        }

        window.draw(playButton);
        window.draw(stopButton);
        window.draw(pauseButton);
        window.display();
    }

    return 0;
}
