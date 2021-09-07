//
// Created by Stefan Annell on 2021-05-09.
//

#pragma once

#include <functional>
#include <vector>

namespace voxie {

    using Action = std::function<void()>;

    struct KeyAction {
        Action action;
    };

    using RegistredKeys = std::vector<KeyAction>;

    struct KeyboardHandler {
        static void processInput();
        static void RegisterAction(const KeyAction&);
    };

}// namespace voxie
