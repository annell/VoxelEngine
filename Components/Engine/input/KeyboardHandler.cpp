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

    bool KeyboardHandler::IsKeyState(const Key &key, const ActionType &actionType) {
        return glfwGetKey(Engine::GetEngine().GetWindow()->GetWindow(), key) == actionType;
    }

    void KeyboardHandler::processInput() {
        if (ImGui::GetIO().WantCaptureKeyboard) {
            return;
        }
        auto &engine = Engine::GetEngine();
        for (auto &keyAction : internal::GetRegisteredKeys()) {
            if (IsKeyState(keyAction.key, ActionType::PRESS)) {
                keyAction.action();
            }
        }
    }

    void KeyboardHandler::RegisterAction(const KeyAction &keyAction) {
        auto &keys = internal::GetRegisteredKeys();
        keys.push_back(keyAction);
    }

}// namespace voxie
