//
// Created by Stefan Annell on 2021-09-25.
//

#pragma once
#include <string>

namespace voxie {

    struct image {
        int width = 0;
        int height = 0;
        int nrChannels = 0;
        unsigned char *data = nullptr;
    };
    using Texture2D = unsigned int;


    image loadImageFromFile(const std::string &file);
    Texture2D loadTextureFromFile(const std::string &file);

}// namespace voxie
