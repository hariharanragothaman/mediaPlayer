#include "media.h"

#include <SFML/Audio.hpp>

#include <iostream>
#include <string>

Media::Media() {}
Media::Media(const std::string &file_path): file_path(file_path) {}

void Media::displayInfo() const
{
    std::cout << "Generic Media File: " << file_path << std::endl;
}
