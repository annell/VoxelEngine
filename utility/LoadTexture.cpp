//
// Created by Stefan Annell on 2021-09-25.
//
#include "LoadTexture.h"
#include <glfw3.h>
#include <stb_image.h>

namespace voxie {

image loadImageFromFile(const std::string &file) {
    int width, height, nrChannels;
    unsigned char *image = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
    return { width, height, nrChannels, image };
}

Texture2D loadTextureFromFile(const std::string &file) {
    voxie::Texture2D texture;
    glGenTextures(1, &texture);
    assert(texture && "Can't be 0");
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto image = loadImageFromFile(file);
    assert(image.data && "No image found!");

    if (image.nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
    } else if (image.nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
        assert(!"Unsupported color format");
    }

    stbi_image_free(image.data);

    return texture;
}

}
