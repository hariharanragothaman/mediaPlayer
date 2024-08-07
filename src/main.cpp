#include <iostream>
#include <string>

#include "audio.h"
#include "loader.h"
#include "mediaManager.h"


int main()
{
    MediaManager manager;
    bool running = true;
    std::string input;

    while (running)
    {
        std::cout << "Enter command (add/play/stop/exit): ";
        std::cin >> input;

        if (input == "add")
        {
            std::string file_path, type;
            std::cout << "Enter file path: ";
            std::cin >> file_path;
            std::cout << "Enter media type (audio/video): ";
            std::cin >> type;
            manager.addMediaToQueue(file_path, type);
        }
        else if (input == "play")
        {
            manager.playNext();
        }
        else if (input == "exit")
        {
            running = false;
        }
        else
        {
            std::cout << "Unknown command." << std::endl;
        }
    }
    return 0;
}
