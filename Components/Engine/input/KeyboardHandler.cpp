//
// Created by Stefan Annell on 2021-05-09.
//

#include "KeyboardHandler.h"
#include "Core.h"
#include <GLFW/glfw3.h>

namespace voxie {

    namespace internal {
        KeyboardHandler::RegistredKeys &GetRegisteredKeys() {
            static KeyboardHandler::RegistredKeys registeredKeys;

            return registeredKeys;
        }

    }// namespace internal

    void KeyboardHandler::processInput() {
        auto& engine = Engine::GetEngine();
        for (auto &key : internal::GetRegisteredKeys()) {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), key.key) == GLFW_PRESS) {
                key.action();
            }
        }
    }

    void KeyboardHandler::RegisterAction(const KeyAction &keyAction) {
        auto &keys = internal::GetRegisteredKeys();
        keys.push_back(keyAction);
    }

}// namespace voxie
