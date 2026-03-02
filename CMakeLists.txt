cmake_minimum_required(VERSION 3.22)
project(mediaPlayer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

target_link_libraries(media_player PRIVATE sfml-audio sfml-graphics)

file(GLOB ASSET_FILES "${CMAKE_SOURCE_DIR}/assets/*")
file(COPY ${ASSET_FILES} DESTINATION ${CMAKE_BINARY_DIR})
