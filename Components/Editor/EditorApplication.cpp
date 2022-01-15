#include "Core.h"
#include "gui/Panel.h"


int main() {
    auto &engine = voxie::Engine::GetEngine();
    engine.Init();

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

    voxie::MouseHandler::RegisterAction({[&engine]() {
        voxie::MouseHandler::UnlockCamera();
    }, voxie::MouseButton::BUTTON_2, voxie::ActionType::RELEASE});

    voxie::MouseHandler::RegisterAction({[&engine]() {
        voxie::MouseHandler::LockCamera();
    }, voxie::MouseButton::BUTTON_2, voxie::ActionType::PRESS});

    engine.onTick.Bind([&](float deltaTime) {
        gui::ShowSceneOverview();
    });
    engine.StartLoop();
    return 0;
}
