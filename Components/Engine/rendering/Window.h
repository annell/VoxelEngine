//
// Created by Stefan Annell on 2021-05-15.
//

#pragma once

#include <string>

class GLFWwindow;

namespace voxie {

    class Window {
    public:
        Window(GLFWwindow *window, unsigned int width, unsigned int height);
        GLFWwindow *GetWindow() const;
        unsigned int GetWidth() const;
        void SetWidth(unsigned int widthIn);
        void SetHeight(unsigned int heightIn);
        unsigned int GetHeight() const;
        void SetTitle(const std::string &);
        void CloseWindow() const;

    private:
        unsigned int width = 1024;
        unsigned int height = 768;
        GLFWwindow *window = nullptr;
        std::string title;
    };

}// namespace voxie
