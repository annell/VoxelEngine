#include "Window.h"
#include <GLFW/glfw3.h>

namespace voxie {

    Window::Window(GLFWwindow *window, unsigned int width, unsigned int height)
        : window(window), width(width), height(height) {
    }
    GLFWwindow *Window::GetWindow() const {
        return window;
    }
    unsigned int Window::GetWidth() const {
        return width;
    }
    void Window::SetWidth(unsigned int widthIn) {
        width = widthIn;
    }
    void Window::SetHeight(unsigned int heightIn) {
        height = heightIn;
    }
    unsigned int Window::GetHeight() const {
        return height;
    }

}// namespace voxie
