//
// Created by Stefan Annell on 2021-05-15.
//

#pragma once

#include <GLFW/glfw3.h>

namespace voxie {

    class Window {
    public:
        Window(GLFWwindow *window, unsigned int width, unsigned int height)
            : window(window), width(width), height(height) {
        }
        [[nodiscard]] GLFWwindow *GetWindow() const { return window; }
        [[nodiscard]] unsigned int GetWidth() const { return width; }
        void SetWidth(unsigned int widthIn) { width = widthIn; }
        void SetHeight(unsigned int heightIn) { height = heightIn; }
        [[nodiscard]] unsigned int GetHeight() const { return height; }

    private:
        unsigned int width = 1024;
        unsigned int height = 768;
        GLFWwindow *window = nullptr;
    };

}// namespace voxie
