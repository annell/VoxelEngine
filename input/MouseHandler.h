//
// Created by Stefan Annell on 2021-05-09.
//

#pragma once

#include <glfw3.h>
struct MouseHandler {
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void UnlockCamera();
    static void LockCamera();
};


