cmake_minimum_required(VERSION 3.22)
project(pycpplink)
set(CMAKE_CXX_STANDARD 20)

set(SFML_INCLUDE_DIR "/opt/homebrew/include/SFML")
include_directories(${SFML_INCLUDE_DIR})

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(media_player src/media.cpp
                            src/main.cpp
                            src/audio.cpp
                            src/loader.cpp
                            src/player.cpp
                            src/queue.cpp
                            src/mediaManager.cpp)

target_link_libraries(media_player PRIVATE sfml-audio)
target_link_libraries(media_player PRIVATE sfml-graphics)
target_compile_features(media_player PRIVATE cxx_std_17)
