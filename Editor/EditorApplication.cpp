#include "Camera.h"
#include "Engine.h"
#include "Factory.h"
#include "KeyboardHandler.h"
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
        if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            engine.GetCamera()->ProcessKeyboard(voxie::FORWARD, engine.GetDeltaTime());
    }});

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
        if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            engine.GetCamera()->ProcessKeyboard(voxie::BACKWARD, engine.GetDeltaTime());
    }});

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
        if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            engine.GetCamera()->ProcessKeyboard(voxie::LEFT, engine.GetDeltaTime());
    }});

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
        if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
            engine.GetCamera()->ProcessKeyboard(voxie::RIGHT, engine.GetDeltaTime());
    }});

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
        if (glfwGetMouseButton(engine.GetWindow()->GetWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE)
            voxie::MouseHandler::UnlockCamera();
    }});

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
        if (glfwGetMouseButton(engine.GetWindow()->GetWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
            voxie::MouseHandler::LockCamera();
    }});

    engine.onTick.Bind([&](float deltaTime) {
        gui::ShowSceneOverview();
        gui::ShowSimpleOverlay("Time per frame: " + std::to_string(FilterSample(deltaTime)));

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
        voxie::helper::RenderingEnd();
    });
    engine.StartLoop();
    return 0;
}
