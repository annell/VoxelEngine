//
// Created by Stefan Annell on 2021-05-15.
//

#pragma once

#include <glfw3.h>

namespace engine::rendering {

class Window {
public:
    Window(GLFWwindow* window, unsigned int width, unsigned int height)
     : window(window), width(width), height(height) {

    }
    GLFWwindow* GetWindow() const { return window; }
    unsigned int GetWidth() const { return width; }
    void SetWidth(unsigned int width) { width = width; }
    void SetHeight(unsigned int height) { height = height; }
    unsigned int GetHeight() const { return height; }

private:
    const unsigned int width = 1024;
    const unsigned int height = 768;
    GLFWwindow* window = nullptr;
};

}


