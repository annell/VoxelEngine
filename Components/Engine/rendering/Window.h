//
// Created by Stefan Annell on 2021-05-15.
//

#pragma once

class GLFWwindow;

namespace voxie {

    class Window {
    public:
        Window(GLFWwindow *window, unsigned int width, unsigned int height);
        [[nodiscard]] GLFWwindow *GetWindow() const;
        [[nodiscard]] unsigned int GetWidth() const;
        void SetWidth(unsigned int widthIn);
        void SetHeight(unsigned int heightIn);
        [[nodiscard]] unsigned int GetHeight() const;

    private:
        unsigned int width = 1024;
        unsigned int height = 768;
        GLFWwindow *window = nullptr;
    };

}// namespace voxie
