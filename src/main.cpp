#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cmath>
#include "mediaManager.h"

static std::string assetPath(const std::string& filename, const std::string& exeDir)
{
    auto path = std::filesystem::path(exeDir) / filename;
    if (std::filesystem::exists(path))
        return path.string();
    return filename;
}

static std::string formatTime(float seconds)
{
    int totalSec = static_cast<int>(seconds);
    int min = totalSec / 60;
    int sec = totalSec % 60;
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << min << ":"
       << std::setw(2) << std::setfill('0') << sec;
    return ss.str();
}

static std::string truncateText(const std::string& text, const sf::Font& font, unsigned int charSize, float maxWidth)
{
    sf::Text measure;
    measure.setFont(font);
    measure.setCharacterSize(charSize);
    measure.setString(text);
    if (measure.getLocalBounds().width <= maxWidth)
        return text;

    for (int len = static_cast<int>(text.size()) - 1; len > 0; --len)
    {
        measure.setString(text.substr(0, len) + "...");
        if (measure.getLocalBounds().width <= maxWidth)
            return text.substr(0, len) + "...";
    }
    return "...";
}

struct FolderBrowser
{
    bool active = false;
    std::string currentPath;
    std::vector<std::filesystem::path> entries;
    int scrollOffset = 0;
    int selectedIndex = -1;
    static constexpr int VISIBLE_ROWS = 12;
    static constexpr float ROW_HEIGHT = 22.f;

    void open(const std::string& startPath)
    {
        active = true;
        navigate(startPath);
    }

    void close()
    {
        active = false;
        entries.clear();
        selectedIndex = -1;
        scrollOffset = 0;
    }

    void navigate(const std::string& path)
    {
        currentPath = path;
        entries.clear();
        scrollOffset = 0;
        selectedIndex = -1;

        try
        {
            auto canonical = std::filesystem::canonical(path);
            currentPath = canonical.string();

            if (canonical.has_parent_path() && canonical != canonical.root_path())
            {
                entries.push_back(canonical.parent_path());
            }

            std::vector<std::filesystem::path> dirs, files;
            for (const auto& entry : std::filesystem::directory_iterator(canonical))
            {
                try
                {
                    if (entry.is_directory() && entry.path().filename().string()[0] != '.')
                        dirs.push_back(entry.path());
                }
                catch (...) {}
            }
            std::sort(dirs.begin(), dirs.end());
            for (const auto& d : dirs)
                entries.push_back(d);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Cannot browse: " << path << " — " << e.what() << std::endl;
        }
    }

    bool hasAudioFiles() const
    {
        try
        {
            for (const auto& entry : std::filesystem::directory_iterator(currentPath))
            {
                if (!entry.is_regular_file()) continue;
                auto ext = entry.path().extension().string();
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                if (ext == ".mp3" || ext == ".ogg" || ext == ".wav" || ext == ".flac")
                    return true;
            }
        }
        catch (...) {}
        return false;
    }

    std::string getDisplayName(int index) const
    {
        if (index < 0 || index >= static_cast<int>(entries.size()))
            return "";
        auto canonical = std::filesystem::canonical(currentPath);
        if (index == 0 && canonical.has_parent_path() && canonical != canonical.root_path())
            return ".. (up)";
        return entries[index].filename().string() + "/";
    }
};

int main(int argc, char* argv[])
{
    std::string exeDir = std::filesystem::canonical(argv[0]).parent_path().string();

    MediaManager manager;

    std::string homeDir = "/tmp";
    const char* home = std::getenv("HOME");
    if (home)
        homeDir = std::string(home);

    std::string mediaDirectory = homeDir;
    if (argc > 1)
    {
        mediaDirectory = argv[1];
    }
    else
    {
        std::string musicDir = homeDir + "/Music";
        if (std::filesystem::exists(musicDir))
            mediaDirectory = musicDir;
    }

    manager.loadDirectory(mediaDirectory);

    const int WINDOW_W = 600;
    const int WINDOW_H = 650;

    const int numBars = 64;
    std::vector<sf::RectangleShape> bars(numBars);
    std::vector<float> barTargets(numBars, 0.f);
    std::vector<float> barCurrents(numBars, 0.f);

    for (int i = 0; i < numBars; ++i)
    {
        bars[i].setSize(sf::Vector2f(7, 1));
        bars[i].setFillColor(sf::Color(180, 50, 255));
        bars[i].setPosition(static_cast<float>(i * 9 + 20), 560.f);
    }

    sf::Font font;
    if (!font.loadFromFile(assetPath("arial.ttf", exeDir)))
    {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

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

    float btnY = 110.f;
    float btnStartX = 170.f;
    float btnSpacing = 60.f;
    playButton.setPosition(btnStartX, btnY);
    stopButton.setPosition(btnStartX + btnSpacing, btnY);
    pauseButton.setPosition(btnStartX + 2 * btnSpacing, btnY);
    loadMusicButton.setPosition(btnStartX + 3 * btnSpacing, btnY);
    nextTrackButton.setPosition(btnStartX + 4 * btnSpacing, btnY);

    const float PROGRESS_X = 50.f;
    const float PROGRESS_Y = 200.f;
    const float PROGRESS_W = 500.f;
    const float PROGRESS_H = 12.f;

    sf::RectangleShape progressBg(sf::Vector2f(PROGRESS_W, PROGRESS_H));
    progressBg.setFillColor(sf::Color(50, 50, 50));
    progressBg.setPosition(PROGRESS_X, PROGRESS_Y);

    sf::RectangleShape progressFill(sf::Vector2f(0, PROGRESS_H));
    progressFill.setFillColor(sf::Color(100, 220, 100));
    progressFill.setPosition(PROGRESS_X, PROGRESS_Y);

    sf::CircleShape progressKnob(7.f);
    progressKnob.setFillColor(sf::Color::White);
    progressKnob.setOrigin(7.f, 7.f);

    const float VOLUME_X = 50.f;
    const float VOLUME_Y = 240.f;
    const float VOLUME_W = 120.f;
    const float VOLUME_H = 8.f;

    sf::RectangleShape volumeBg(sf::Vector2f(VOLUME_W, VOLUME_H));
    volumeBg.setFillColor(sf::Color(50, 50, 50));
    volumeBg.setPosition(VOLUME_X, VOLUME_Y);

    sf::RectangleShape volumeFill(sf::Vector2f(VOLUME_W, VOLUME_H));
    volumeFill.setFillColor(sf::Color(80, 160, 255));
    volumeFill.setPosition(VOLUME_X, VOLUME_Y);

    sf::CircleShape volumeKnob(5.f);
    volumeKnob.setFillColor(sf::Color::White);
    volumeKnob.setOrigin(5.f, 5.f);

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(11);
    titleText.setFillColor(sf::Color(200, 200, 200));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(50, 15);
    titleText.setString("mediaPlayer");

    sf::Text trackNameText;
    trackNameText.setFont(font);
    trackNameText.setCharacterSize(16);
    trackNameText.setFillColor(sf::Color::White);
    trackNameText.setPosition(50, 55);

    sf::Text trackIndexText;
    trackIndexText.setFont(font);
    trackIndexText.setCharacterSize(12);
    trackIndexText.setFillColor(sf::Color(150, 150, 150));
    trackIndexText.setPosition(50, 80);

    sf::Text currentTimeText;
    currentTimeText.setFont(font);
    currentTimeText.setCharacterSize(11);
    currentTimeText.setFillColor(sf::Color::White);
    currentTimeText.setPosition(PROGRESS_X, PROGRESS_Y - 18);

    sf::Text totalTimeText;
    totalTimeText.setFont(font);
    totalTimeText.setCharacterSize(11);
    totalTimeText.setFillColor(sf::Color(150, 150, 150));
    totalTimeText.setPosition(PROGRESS_X + PROGRESS_W - 40, PROGRESS_Y - 18);

    sf::Text volumeLabel;
    volumeLabel.setFont(font);
    volumeLabel.setCharacterSize(11);
    volumeLabel.setFillColor(sf::Color(150, 150, 150));
    volumeLabel.setPosition(VOLUME_X + VOLUME_W + 10, VOLUME_Y - 3);

    sf::Text shortcutHint;
    shortcutHint.setFont(font);
    shortcutHint.setCharacterSize(10);
    shortcutHint.setFillColor(sf::Color(80, 80, 80));
    shortcutHint.setPosition(50, WINDOW_H - 25.f);
    shortcutHint.setString("[Space] Play/Pause  [S] Stop  [N] Next  [P] Prev  [Up/Down] Volume  [L] Load Folder");

    const float PLAYLIST_X = 50.f;
    const float PLAYLIST_Y = 280.f;
    const float PLAYLIST_W = 500.f;
    const float PLAYLIST_ROW_H = 22.f;
    const int PLAYLIST_VISIBLE = 12;

    sf::RectangleShape playlistBg(sf::Vector2f(PLAYLIST_W, PLAYLIST_ROW_H * PLAYLIST_VISIBLE));
    playlistBg.setFillColor(sf::Color(25, 25, 25));
    playlistBg.setPosition(PLAYLIST_X, PLAYLIST_Y);

    sf::Text playlistHeader;
    playlistHeader.setFont(font);
    playlistHeader.setCharacterSize(11);
    playlistHeader.setFillColor(sf::Color(100, 100, 100));
    playlistHeader.setPosition(PLAYLIST_X, PLAYLIST_Y - 16);

    int playlistScrollOffset = 0;
    FolderBrowser browser;
    float currentVolume = 100.f;
    bool draggingProgress = false;
    bool draggingVolume = false;

    sf::Clock vizClock;

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "mediaPlayer", sf::Style::Close);
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        manager.checkAutoAdvance();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (browser.active)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                        browser.close();
                }
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    float mx = static_cast<float>(event.mouseButton.x);
                    float my = static_cast<float>(event.mouseButton.y);

                    sf::FloatRect loadBtnRect(PLAYLIST_X + PLAYLIST_W - 120, PLAYLIST_Y - 18, 120, 16);
                    if (loadBtnRect.contains(mx, my) && browser.hasAudioFiles())
                    {
                        manager.loadDirectory(browser.currentPath);
                        browser.close();
                        continue;
                    }

                    float listTop = PLAYLIST_Y;
                    float listBot = PLAYLIST_Y + FolderBrowser::VISIBLE_ROWS * FolderBrowser::ROW_HEIGHT;
                    if (my >= listTop && my < listBot && mx >= PLAYLIST_X && mx < PLAYLIST_X + PLAYLIST_W)
                    {
                        int row = static_cast<int>((my - listTop) / FolderBrowser::ROW_HEIGHT);
                        int idx = row + browser.scrollOffset;
                        if (idx >= 0 && idx < static_cast<int>(browser.entries.size()))
                        {
                            browser.navigate(browser.entries[idx].string());
                        }
                    }
                }
                if (event.type == sf::Event::MouseWheelScrolled)
                {
                    int maxScroll = std::max(0, static_cast<int>(browser.entries.size()) - FolderBrowser::VISIBLE_ROWS);
                    browser.scrollOffset -= static_cast<int>(event.mouseWheelScroll.delta) * 2;
                    browser.scrollOffset = std::max(0, std::min(browser.scrollOffset, maxScroll));
                }
                continue;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                    manager.togglePlayPause();
                else if (event.key.code == sf::Keyboard::S)
                    manager.stopMedia();
                else if (event.key.code == sf::Keyboard::N || event.key.code == sf::Keyboard::Right)
                {
                    manager.stopMedia();
                    manager.playNext();
                }
                else if (event.key.code == sf::Keyboard::P || event.key.code == sf::Keyboard::Left)
                {
                    manager.stopMedia();
                    manager.playPrevious();
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    currentVolume = std::min(100.f, currentVolume + 5.f);
                    manager.setVolume(currentVolume);
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    currentVolume = std::max(0.f, currentVolume - 5.f);
                    manager.setVolume(currentVolume);
                }
                else if (event.key.code == sf::Keyboard::L)
                {
                    browser.open(homeDir);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                float mx = static_cast<float>(event.mouseButton.x);
                float my = static_cast<float>(event.mouseButton.y);

                if (playButton.getGlobalBounds().contains(mx, my))
                {
                    if (manager.isPaused())
                        manager.resumeMedia();
                    else if (!manager.isPlaying())
                        manager.playNext();
                }
                else if (stopButton.getGlobalBounds().contains(mx, my))
                {
                    manager.stopMedia();
                }
                else if (pauseButton.getGlobalBounds().contains(mx, my))
                {
                    manager.pauseMedia();
                }
                else if (nextTrackButton.getGlobalBounds().contains(mx, my))
                {
                    manager.stopMedia();
                    manager.playNext();
                }
                else if (loadMusicButton.getGlobalBounds().contains(mx, my))
                {
                    browser.open(homeDir);
                }

                sf::FloatRect progressRect(PROGRESS_X, PROGRESS_Y - 5, PROGRESS_W, PROGRESS_H + 10);
                if (progressRect.contains(mx, my))
                {
                    draggingProgress = true;
                    float ratio = (mx - PROGRESS_X) / PROGRESS_W;
                    float totalSec = manager.getTotalTimeSeconds();
                    if (totalSec > 0.f)
                        manager.seekTo(ratio * totalSec);
                }

                sf::FloatRect volumeRect(VOLUME_X, VOLUME_Y - 5, VOLUME_W, VOLUME_H + 10);
                if (volumeRect.contains(mx, my))
                {
                    draggingVolume = true;
                    currentVolume = ((mx - VOLUME_X) / VOLUME_W) * 100.f;
                    currentVolume = std::max(0.f, std::min(100.f, currentVolume));
                    manager.setVolume(currentVolume);
                }

                float listTop = PLAYLIST_Y;
                float listBot = PLAYLIST_Y + PLAYLIST_ROW_H * PLAYLIST_VISIBLE;
                if (my >= listTop && my < listBot && mx >= PLAYLIST_X && mx < PLAYLIST_X + PLAYLIST_W)
                {
                    int row = static_cast<int>((my - listTop) / PLAYLIST_ROW_H);
                    int idx = row + playlistScrollOffset;
                    if (idx >= 0 && idx < manager.getTrackCount())
                    {
                        manager.stopMedia();
                        manager.playTrackAt(idx);
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                draggingProgress = false;
                draggingVolume = false;
            }

            if (event.type == sf::Event::MouseMoved && (draggingProgress || draggingVolume))
            {
                float mx = static_cast<float>(event.mouseMove.x);
                if (draggingProgress)
                {
                    float ratio = (mx - PROGRESS_X) / PROGRESS_W;
                    ratio = std::max(0.f, std::min(1.f, ratio));
                    float totalSec = manager.getTotalTimeSeconds();
                    if (totalSec > 0.f)
                        manager.seekTo(ratio * totalSec);
                }
                if (draggingVolume)
                {
                    currentVolume = ((mx - VOLUME_X) / VOLUME_W) * 100.f;
                    currentVolume = std::max(0.f, std::min(100.f, currentVolume));
                    manager.setVolume(currentVolume);
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled)
            {
                int maxScroll = std::max(0, manager.getTrackCount() - PLAYLIST_VISIBLE);
                playlistScrollOffset -= static_cast<int>(event.mouseWheelScroll.delta) * 2;
                playlistScrollOffset = std::max(0, std::min(playlistScrollOffset, maxScroll));
            }
        }

        if (vizClock.getElapsedTime().asMilliseconds() > 80)
        {
            if (manager.isPlaying() && !manager.isPaused())
            {
                for (int i = 0; i < numBars; ++i)
                {
                    barTargets[i] = (static_cast<float>(std::rand()) / RAND_MAX) * 130.f;
                }
            }
            else
            {
                for (int i = 0; i < numBars; ++i)
                    barTargets[i] = 1.f;
            }
            vizClock.restart();
        }

        for (int i = 0; i < numBars; ++i)
        {
            barCurrents[i] += (barTargets[i] - barCurrents[i]) * 0.25f;
            bars[i].setSize(sf::Vector2f(7, barCurrents[i]));
            bars[i].setPosition(bars[i].getPosition().x, 560.f - barCurrents[i]);
        }

        float curSec = manager.getCurrentTimeSeconds();
        float totSec = manager.getTotalTimeSeconds();
        float progress = (totSec > 0.f) ? (curSec / totSec) : 0.f;

        progressFill.setSize(sf::Vector2f(PROGRESS_W * progress, PROGRESS_H));
        progressKnob.setPosition(PROGRESS_X + PROGRESS_W * progress, PROGRESS_Y + PROGRESS_H / 2.f);

        currentTimeText.setString(formatTime(curSec));
        totalTimeText.setString(formatTime(totSec));

        float volRatio = currentVolume / 100.f;
        volumeFill.setSize(sf::Vector2f(VOLUME_W * volRatio, VOLUME_H));
        volumeKnob.setPosition(VOLUME_X + VOLUME_W * volRatio, VOLUME_Y + VOLUME_H / 2.f);
        volumeLabel.setString("Vol " + std::to_string(static_cast<int>(currentVolume)) + "%");

        std::string trackName = manager.getCurrentTrackName();
        if (!trackName.empty())
        {
            trackNameText.setString(truncateText(trackName, font, 16, PROGRESS_W));
            int idx = manager.getCurrentTrackIndex() + 1;
            int total = manager.getTrackCount();
            trackIndexText.setString("Track " + std::to_string(idx) + " / " + std::to_string(total));
        }
        else
        {
            trackNameText.setString("No track loaded");
            trackIndexText.setString(std::to_string(manager.getTrackCount()) + " tracks in playlist");
        }

        playlistHeader.setString("PLAYLIST (" + std::to_string(manager.getTrackCount()) + " tracks)");

        window.clear(sf::Color(18, 18, 22));

        sf::RectangleShape topBar(sf::Vector2f(static_cast<float>(WINDOW_W), 40.f));
        topBar.setFillColor(sf::Color(28, 28, 34));
        window.draw(topBar);
        window.draw(titleText);

        window.draw(trackNameText);
        window.draw(trackIndexText);

        window.draw(playButton);
        window.draw(stopButton);
        window.draw(pauseButton);
        window.draw(loadMusicButton);
        window.draw(nextTrackButton);

        window.draw(currentTimeText);
        window.draw(totalTimeText);
        window.draw(progressBg);
        window.draw(progressFill);
        window.draw(progressKnob);

        window.draw(volumeBg);
        window.draw(volumeFill);
        window.draw(volumeKnob);
        window.draw(volumeLabel);

        window.draw(playlistHeader);
        window.draw(playlistBg);

        const auto& tracks = manager.getTracks();
        int currentIdx = manager.getCurrentTrackIndex();
        for (int i = 0; i < PLAYLIST_VISIBLE; ++i)
        {
            int trackIdx = i + playlistScrollOffset;
            if (trackIdx >= static_cast<int>(tracks.size()))
                break;

            float rowY = PLAYLIST_Y + i * PLAYLIST_ROW_H;

            if (trackIdx == currentIdx && manager.isPlaying())
            {
                sf::RectangleShape highlight(sf::Vector2f(PLAYLIST_W, PLAYLIST_ROW_H));
                highlight.setFillColor(sf::Color(40, 40, 55));
                highlight.setPosition(PLAYLIST_X, rowY);
                window.draw(highlight);
            }

            sf::Text rowText;
            rowText.setFont(font);
            rowText.setCharacterSize(12);
            rowText.setPosition(PLAYLIST_X + 8, rowY + 3);

            if (trackIdx == currentIdx && manager.isPlaying())
                rowText.setFillColor(sf::Color(100, 220, 100));
            else
                rowText.setFillColor(sf::Color(170, 170, 170));

            std::string label = std::to_string(trackIdx + 1) + ".  " + tracks[trackIdx]->getFileName();
            rowText.setString(truncateText(label, font, 12, PLAYLIST_W - 16));
            window.draw(rowText);
        }

        for (const auto& bar : bars)
            window.draw(bar);

        window.draw(shortcutHint);

        if (browser.active)
        {
            sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(WINDOW_W), static_cast<float>(WINDOW_H)));
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);

            sf::RectangleShape browserBg(sf::Vector2f(PLAYLIST_W + 20, FolderBrowser::VISIBLE_ROWS * FolderBrowser::ROW_HEIGHT + 60));
            browserBg.setFillColor(sf::Color(30, 30, 38));
            browserBg.setOutlineColor(sf::Color(80, 80, 100));
            browserBg.setOutlineThickness(1.f);
            browserBg.setPosition(PLAYLIST_X - 10, PLAYLIST_Y - 40);
            window.draw(browserBg);

            sf::Text browserTitle;
            browserTitle.setFont(font);
            browserTitle.setCharacterSize(13);
            browserTitle.setFillColor(sf::Color::White);
            browserTitle.setStyle(sf::Text::Bold);
            browserTitle.setPosition(PLAYLIST_X, PLAYLIST_Y - 32);
            browserTitle.setString("Select Folder");
            window.draw(browserTitle);

            sf::Text pathLabel;
            pathLabel.setFont(font);
            pathLabel.setCharacterSize(10);
            pathLabel.setFillColor(sf::Color(120, 120, 140));
            pathLabel.setPosition(PLAYLIST_X, PLAYLIST_Y - 16);
            pathLabel.setString(truncateText(browser.currentPath, font, 10, PLAYLIST_W - 130));
            window.draw(pathLabel);

            if (browser.hasAudioFiles())
            {
                sf::RectangleShape loadBtn(sf::Vector2f(120, 16));
                loadBtn.setFillColor(sf::Color(60, 140, 60));
                loadBtn.setPosition(PLAYLIST_X + PLAYLIST_W - 120, PLAYLIST_Y - 18);
                window.draw(loadBtn);

                sf::Text loadLabel;
                loadLabel.setFont(font);
                loadLabel.setCharacterSize(10);
                loadLabel.setFillColor(sf::Color::White);
                loadLabel.setPosition(PLAYLIST_X + PLAYLIST_W - 105, PLAYLIST_Y - 17);
                loadLabel.setString("Load This Folder");
                window.draw(loadLabel);
            }

            for (int i = 0; i < FolderBrowser::VISIBLE_ROWS; ++i)
            {
                int idx = i + browser.scrollOffset;
                if (idx >= static_cast<int>(browser.entries.size()))
                    break;

                float rowY = PLAYLIST_Y + i * FolderBrowser::ROW_HEIGHT;

                sf::Text entry;
                entry.setFont(font);
                entry.setCharacterSize(12);
                entry.setFillColor(sf::Color(190, 190, 210));
                entry.setPosition(PLAYLIST_X + 8, rowY + 2);
                entry.setString(truncateText(browser.getDisplayName(idx), font, 12, PLAYLIST_W - 16));
                window.draw(entry);
            }

            sf::Text escHint;
            escHint.setFont(font);
            escHint.setCharacterSize(10);
            escHint.setFillColor(sf::Color(80, 80, 100));
            escHint.setPosition(PLAYLIST_X, PLAYLIST_Y + FolderBrowser::VISIBLE_ROWS * FolderBrowser::ROW_HEIGHT + 8);
            escHint.setString("[Esc] Close    [Click] Navigate    [Scroll] Browse");
            window.draw(escHint);
        }

        window.display();
    }

    return 0;
}
