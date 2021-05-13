//
// Created by Stefan Annell on 2021-05-09.
//

#pragma once
#include <glew.h>
#include <glfw3.h>

#include <vector>
#include <functional>

namespace engine::input {

using Action = std::function<void()>;

struct KeyAction {
    Action action;
};

using RegistredKeys = std::vector<KeyAction>;

struct KeyboardHandler {
    static void processInput();
    static void RegisterAction(KeyAction);
};

}
