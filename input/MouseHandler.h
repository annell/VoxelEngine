//
// Created by Stefan Annell on 2021-05-09.
//

#pragma once

class GLFWwindow;
namespace voxie::MouseHandler {

    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    void UnlockCamera();
    void LockCamera();

}// namespace voxie::MouseHandler
