//
// Created by Stefan Annell on 2022-01-30.
//

#include "EditorGameMode.h"
#include "Core.h"
#include <gui/Panel.h>

namespace voxie {

EditorGameMode::EditorGameMode()
: voxie::GameMode() {
    Initialize();
}

void EditorGameMode::Initialize() {
    auto& engine = voxie::Engine::GetEngine();
    auto& scene = engine.GetScene();

    voxie::KeyboardHandler::RegisterAction({[&]() {
        MouseHandler::MovementLock();
    }, voxie::Key::KEY_LEFT_SHIFT});

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
      engine.GetWindow()->CloseWindow();
    }, voxie::Key::KEY_ESCAPE});

    voxie::MouseHandler::RegisterAction({[]() {
      voxie::MouseHandler::UnlockCamera();
    }, voxie::MouseButton::BUTTON_2, voxie::ActionType::RELEASE});

    voxie::MouseHandler::RegisterAction({[]() {
      voxie::MouseHandler::LockCamera();
    }, voxie::MouseButton::BUTTON_2, voxie::ActionType::PRESS});

}

void EditorGameMode::OnTick(float deltaTime) {
    voxie::GameMode::OnTick(deltaTime);
    gui::ShowSceneOverview();
}

}

