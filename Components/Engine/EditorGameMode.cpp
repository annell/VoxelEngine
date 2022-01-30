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

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
      engine.GetCamera()->ProcessKeyboard(voxie::FORWARD, engine.GetDeltaTime());
    }, voxie::Key::KEY_W});

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
      engine.GetCamera()->ProcessKeyboard(voxie::BACKWARD, engine.GetDeltaTime());
    }, voxie::Key::KEY_S});

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
      engine.GetCamera()->ProcessKeyboard(voxie::LEFT, engine.GetDeltaTime());
    }, voxie::Key::KEY_A});

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
      engine.GetCamera()->ProcessKeyboard(voxie::RIGHT, engine.GetDeltaTime());
    }, voxie::Key::KEY_D});

    voxie::KeyboardHandler::RegisterAction({[&]() {
      hideUI = true;
    }, voxie::Key::KEY_Q});

    voxie::KeyboardHandler::RegisterAction({[&]() {
      hideUI = false;
    }, voxie::Key::KEY_E});

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
    if (!hideUI) {
        gui::ShowSceneOverview();
        gui::ShowPlayPauseControls();
    }
}

}

