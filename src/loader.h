//
// Created by Hariharan Ragothaman on 8/7/24.
//

#ifndef PYCPPLINK_LOADER_H
#define PYCPPLINK_LOADER_H
#include "media.h"

class Loader
{
public:
    std::unique_ptr<Media> loadMedia(const std::string file_path, const std::string& type);
};

#endif //PYCPPLINK_LOADER_H
