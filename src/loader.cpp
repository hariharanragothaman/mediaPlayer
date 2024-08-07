#include "loader.h"
#include "audio.h"
#include <memory>

/*
 *  Using unique_ptr to manage media files
 *  Each file is loaded into a unique pointer and is freed, when media is no longer needed
 */

std::unique_ptr<Media> Loader::loadMedia(const std::string file_path, const std::string& type)
{
    if (type == "audio")
    {
        return std::make_unique<Audio>(file_path);
    }
    return nullptr;
}
