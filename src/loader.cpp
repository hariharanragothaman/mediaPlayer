#include "loader.h"
#include "audio.h"

std::unique_ptr<Media> Loader::loadMedia(const std::string& file_path, const std::string& type)
{
    if (type == "audio")
    {
        return std::make_unique<Audio>(file_path);
    }
    return nullptr;
}
