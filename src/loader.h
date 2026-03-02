#ifndef MEDIAPLAYER_LOADER_H
#define MEDIAPLAYER_LOADER_H
#include "media.h"
#include <memory>

class Loader
{
public:
    std::unique_ptr<Media> loadMedia(const std::string& file_path, const std::string& type);
};

#endif //MEDIAPLAYER_LOADER_H
