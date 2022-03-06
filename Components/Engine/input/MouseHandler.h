//
// Created by Stefan Annell on 2021-05-09.
//

#pragma once

#include "InputDefinitions.h"
#include <functional>
#include <vector>

class GLFWwindow;
namespace voxie {

    struct MouseAction {
        using Action = std::function<void()>;
        Action action;
        MouseButton key;
        ActionType type;
    };

    struct MouseHandler {
        using RegistredKeys = std::vector<MouseAction>;

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
        static void UnlockCamera();
        static void LockCamera();

        static void MovementLock();
        static void MovementUnlock();

        static void CastRay();

        static void processInput();
        static void RegisterAction(const MouseAction &);
    };

}// namespace voxie
