#include "Camera.h"
#include "Engine.h"
#include "Factory.h"
#include "KeyboardHandler.h"
#include "gui/Panel.h"
#include <Sprite.h>


int FilterSample(int new_sample) {
    static int last_result = 0;
    const int smoothing = 5;
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
        if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
            voxie::MouseHandler::UnlockCamera();
    }});

    voxie::KeyboardHandler::RegisterAction({[&engine]() {
        if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            voxie::MouseHandler::LockCamera();
    }});

    engine.onTick.Bind([&](float deltaTime) {
        gui::ShowSceneOverview();
        gui::ShowSimpleOverlay("FPS: " + std::to_string(FilterSample((int) (1 / deltaTime))));

        voxie::helper::RenderingBegin();
        auto sceneObjects = engine.GetScene().GetEntities();
        for (const auto& config : GetRenderingConfigs(engine.GetCamera(), sceneObjects)) {
            voxie::helper::Submit(config);
        }
        for (const auto& entity : sceneObjects) {
            if (auto model = voxie::helper::GetComponent<voxie::Chunk>(entity)) {
                voxie::helper::Submit(model->GetRenderingConfig());
            } else if (auto model = voxie::helper::GetComponent<voxie::Sprite>(entity)) {
                voxie::helper::Submit(model->GetRenderingConfig());
            }
        }
        voxie::helper::RenderingEnd();
    });
    engine.StartLoop();
    return 0;
}
