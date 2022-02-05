//
// Created by Stefan Annell on 2021-05-09.
//

#pragma once

#include <functional>
#include <vector>
#include "InputDefinitions.h"

namespace voxie {
    struct KeyAction {
        using Action = std::function<void()>;
        Action action;
        Key key = KEY_UNKNOWN;
        bool press = true;
    };

    struct KeyboardHandler {
        using RegistredKeys = std::vector<KeyAction>;
        static void processInput();
        static void RegisterAction(const KeyAction &);
        static bool IsKeyState(const Key& key, const ActionType& actionType);
    };

}// namespace voxie
