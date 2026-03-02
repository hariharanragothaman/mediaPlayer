# mediaPlayer

A cross-platform media player built from scratch in C++ using [SFML](https://www.sfml-dev.org/), created as a learning project to understand the internals of media players like VLC.

## Overview

This project implements a media player with a graphical interface, audio playback engine, and media queue management. The goal is to incrementally build toward a feature-rich player while learning about audio processing, multimedia frameworks, and real-time UI rendering.

### Current Features

- Audio playback (MP3) via SFML's audio module
- Play, Pause, Stop, and Next Track controls
- Track progress bar with elapsed time display
- Media queue with sequential playback
- Randomized audio visualization bars

### Architecture

```
src/
├── main.cpp           # SFML window, UI rendering, and event loop
├── media.h/cpp        # Abstract base class for all media types
├── audio.h/cpp        # Audio implementation (wraps sf::Music)
├── player.h/cpp       # Playback engine with threaded playback
├── queue.h/cpp        # Media queue (std::deque-backed)
├── loader.h/cpp       # Factory for creating Media objects from files
└── mediaManager.h/cpp # Coordinator: ties loader, player, and queue together

assets/                # UI assets (icons, fonts)
```

**Key design decisions:**
- `Media` is an abstract base class, making it straightforward to add Video or other media types later
- `Player` runs playback on a dedicated thread so the UI stays responsive
- `MediaManager` acts as a facade, coordinating the loader, queue, and player

## Building

### Prerequisites

- CMake 3.22+
- A C++17 compiler (GCC, Clang, or MSVC)
- SFML dependencies are fetched automatically via CMake's FetchContent (SFML 2.6.x)

### macOS

```bash
brew install cmake
git clone https://github.com/hariharanragothaman/mediaPlayer.git
cd mediaPlayer
cmake -S . -B build
cmake --build build
./build/media_player
```

### Linux (Ubuntu/Debian)

```bash
sudo apt-get install cmake g++ libglu1-mesa-dev freeglut3-dev mesa-common-dev
git clone https://github.com/hariharanragothaman/mediaPlayer.git
cd mediaPlayer
cmake -S . -B build
cmake --build build
./build/media_player
```

### Usage

By default, the player scans `/tmp` for `.mp3` files on startup. Place some MP3 files there, or modify the media directory path in `src/main.cpp`.

## Roadmap

This is a learning project aiming toward VLC-level understanding. Planned areas of exploration:

- [ ] Video playback support
- [ ] Playlist management (load/save, reorder, shuffle)
- [ ] Volume control and equalizer
- [ ] Real audio spectrum visualization (FFT-based)
- [ ] Seek/scrub via progress bar click
- [ ] File browser dialog for loading media
- [ ] Keyboard shortcuts
- [ ] Support for more formats (FLAC, WAV, OGG, MP4, AVI)
- [ ] Subtitle rendering
- [ ] Cross-platform packaging

## License

MIT License -- see [LICENSE](LICENSE) for details.
