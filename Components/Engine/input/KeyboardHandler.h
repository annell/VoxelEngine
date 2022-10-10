//
// Created by Stefan Annell on 2021-05-09.
//

#pragma once

#include "InputDefinitions.h"
#include <functional>
#include <vector>

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
        static bool IsKeyState(const Key &key, const ActionType &actionType);
        static void Reset();
    };

}// namespace voxie
