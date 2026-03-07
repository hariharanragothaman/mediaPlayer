#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include "mediaManager.h"

static std::string assetPath(const std::string& filename, const std::string& exeDir)
{
    auto path = std::filesystem::path(exeDir) / filename;
    if (std::filesystem::exists(path))
        return path.string();
    return filename;
}

int main(int argc, char* argv[])
{
    std::string exeDir = std::filesystem::canonical(argv[0]).parent_path().string();

    MediaManager manager;
    std::string mediaDirectory = "/tmp";

    for(const auto& entry : std::filesystem::directory_iterator(mediaDirectory))
    {
        if (entry.path().extension() == ".mp3")
        {
            manager.addMediaToQueue(entry.path().string(), "audio");
        }
    }

    const int numBars = 64;
    std::vector<sf::RectangleShape> bars(numBars);

    for (int i = 0; i < numBars; ++i) {
        bars[i].setSize(sf::Vector2f(6, 1));
        bars[i].setFillColor(sf::Color::Magenta);
        bars[i].setPosition(i * 7 + 25, 450);
        bars[i].setOrigin(0, 0);
    }

    sf::RectangleShape statusBar(sf::Vector2f(15, 10));
    statusBar.setFillColor(sf::Color::Green);
    statusBar.setPosition(50, 200);

    sf::RectangleShape statusBarBackground(sf::Vector2f(400, 10));
    statusBarBackground.setFillColor(sf::Color(80, 80, 80));
    statusBarBackground.setPosition(50, 200);

    sf::Font font;
    if (!font.loadFromFile(assetPath("arial.ttf", exeDir))) {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    sf::Text currentTimeText;
    currentTimeText.setFont(font);
    currentTimeText.setCharacterSize(12);
    currentTimeText.setFillColor(sf::Color::White);
    currentTimeText.setPosition(50, 180);

    sf::Text totalTimeText;
    totalTimeText.setFont(font);
    totalTimeText.setCharacterSize(12);
    totalTimeText.setFillColor(sf::Color(180, 180, 180));
    totalTimeText.setPosition(410, 180);

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(500, 500), "Media Player");

    sf::Texture playTexture, stopTexture, pauseTexture, loadMusicTexture, nextTrackTexture;
    if (!playTexture.loadFromFile(assetPath("play.png", exeDir)) ||
        !stopTexture.loadFromFile(assetPath("stop.png", exeDir)) ||
        !pauseTexture.loadFromFile(assetPath("pause.png", exeDir)) ||
        !loadMusicTexture.loadFromFile(assetPath("loadMusic.png", exeDir)) ||
        !nextTrackTexture.loadFromFile(assetPath("nextTrack.png", exeDir)))
    {
        std::cerr << "Failed to load button textures" << std::endl;
        return 1;
    }

    sf::Sprite playButton(playTexture);
    sf::Sprite stopButton(stopTexture);
    sf::Sprite pauseButton(pauseTexture);
    sf::Sprite loadMusicButton(loadMusicTexture);
    sf::Sprite nextTrackButton(nextTrackTexture);

    playButton.setPosition(100, 100);
    stopButton.setPosition(160, 100);
    pauseButton.setPosition(220, 100);
    loadMusicButton.setPosition(280, 100);
    nextTrackButton.setPosition(340, 100);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (playButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    if (manager.isPaused())
                    {
                        std::cout << "Resuming playback" << std::endl;
                        manager.resumeMedia();
                    }
                    else if (!manager.isPlaying())
                    {
                        std::cout << "Playing next track" << std::endl;
                        manager.playNext();
                    }
                }
                if (stopButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    std::cout << "Stop" << std::endl;
                    manager.stopMedia();
                }
                if (pauseButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    std::cout << "Pause" << std::endl;
                    manager.pauseMedia();
                }
                if (nextTrackButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    std::cout << "Next track" << std::endl;
                    manager.stopMedia();
                    manager.playNext();
                }
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > 100)
        {
            if (manager.isPlaying() && !manager.isPaused())
            {
                for (int i = 0; i < numBars; ++i)
                {
                    float amplitude = static_cast<float>(std::rand()) / RAND_MAX;
                    bars[i].setSize(sf::Vector2f(6, amplitude * 150));
                    bars[i].setPosition(bars[i].getPosition().x, 450 - bars[i].getSize().y);
                }
            }
            else
            {
                for (int i = 0; i < numBars; ++i)
                {
                    bars[i].setSize(sf::Vector2f(6, 1));
                    bars[i].setPosition(bars[i].getPosition().x, 450);
                }
            }
            clock.restart();
        }

        sf::Time currentTime = manager.getCurrentTime();
        sf::Time totalTime = manager.getTotalTime();

        if (totalTime != sf::Time::Zero)
        {
            float progress = currentTime.asSeconds() / totalTime.asSeconds();
            statusBar.setSize(sf::Vector2f(400 * progress, 10));

            int curMin = static_cast<int>(currentTime.asSeconds()) / 60;
            int curSec = static_cast<int>(currentTime.asSeconds()) % 60;
            int totMin = static_cast<int>(totalTime.asSeconds()) / 60;
            int totSec = static_cast<int>(totalTime.asSeconds()) % 60;

            std::stringstream curStream, totStream;
            curStream << std::setw(2) << std::setfill('0') << curMin << ":"
                      << std::setw(2) << std::setfill('0') << curSec;
            totStream << std::setw(2) << std::setfill('0') << totMin << ":"
                      << std::setw(2) << std::setfill('0') << totSec;

            currentTimeText.setString(curStream.str());
            totalTimeText.setString(totStream.str());
        }
        else
        {
            statusBar.setSize(sf::Vector2f(0, 10));
            currentTimeText.setString("00:00");
            totalTimeText.setString("00:00");
        }

        window.clear(sf::Color::Black);

        for (const auto& bar : bars) {
            window.draw(bar);
        }

        window.draw(playButton);
        window.draw(stopButton);
        window.draw(pauseButton);
        window.draw(loadMusicButton);
        window.draw(nextTrackButton);

        window.draw(statusBarBackground);
        window.draw(statusBar);
        window.draw(currentTimeText);
        window.draw(totalTimeText);

        window.display();
    }

    return 0;
}
