//
// Created by Stefan Annell on 2021-05-09.
//

#include "KeyboardHandler.h"

namespace engine::input {

namespace internal {
RegistredKeys& GetRegisteredKeys() {
    static RegistredKeys registeredKeys;

    return registeredKeys;
}

}

void KeyboardHandler::processInput() {
    for (auto& key : internal::GetRegisteredKeys()) {
        key.action();
    }
}

void KeyboardHandler::RegisterAction(KeyAction keyAction) {
    auto& keys = internal::GetRegisteredKeys();
    keys.push_back(keyAction);
}

}
