//
// Created by Stefan Annell on 2021-05-09.
//

#include "KeyboardHandler.h"

namespace voxie {

    namespace internal {
        RegistredKeys &GetRegisteredKeys() {
            static RegistredKeys registeredKeys;

            return registeredKeys;
        }

    }// namespace internal

    void KeyboardHandler::processInput() {
        for (auto &key : internal::GetRegisteredKeys()) {
            key.action();
        }
    }

    void KeyboardHandler::RegisterAction(const KeyAction &keyAction) {
        auto &keys = internal::GetRegisteredKeys();
        keys.push_back(keyAction);
    }

}// namespace voxie
