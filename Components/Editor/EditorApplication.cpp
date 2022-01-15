#include "Core.h"
#include "gui/Panel.h"

template<typename T>
T FilterSample(T new_sample) {
    static T last_result = 0;
    const T smoothing = 5;
    last_result = (new_sample - last_result) / smoothing + last_result;
    return last_result;
}

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
        //gui::ShowSimpleOverlay("Time per frame: " + std::to_string(FilterSample(deltaTime)));

        voxie::helper::RenderingBegin();
        auto sceneObjects = engine.GetScene().GetEntities();
        for (const auto &config : GetRenderingConfigs(engine.GetCamera(), sceneObjects)) {
            voxie::helper::Submit(config);
        }
        for (const auto &entity : sceneObjects) {
            if (auto model = voxie::helper::GetSceneNode<voxie::Chunk>(entity)) {
                voxie::helper::Submit(model->GetRenderingConfig());
            } else if (auto model = voxie::helper::GetSceneNode<voxie::Sprite>(entity)) {
                voxie::helper::Submit(model->GetRenderingConfig());
            }
        }

        if (auto skybox = engine.GetScene().GetSkybox()) {
            voxie::helper::Submit(skybox->GetRenderingConfig());
        }

        voxie::helper::RenderingEnd();
    });
    engine.StartLoop();
    return 0;
}
