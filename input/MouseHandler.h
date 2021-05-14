//
// Created by Stefan Annell on 2021-05-09.
//

#pragma once

class GLFWwindow;
namespace engine::input {

struct MouseHandler {
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void UnlockCamera();
    static void LockCamera();
};

}
