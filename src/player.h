//
// Created by Hariharan Ragothaman on 8/7/24.
//

#ifndef PYCPPLINK_PLAYER_H
#define PYCPPLINK_PLAYER_H

#include "media.h"

#include <iostream>

class Player
{
private:
    std::shared_ptr<Media> current_media;
public:
    void play(std::shared_ptr<Media> media);
    void stop();
};

#endif //PYCPPLINK_PLAYER_H
