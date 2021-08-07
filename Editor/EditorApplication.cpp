#include "Engine.h"
#include "Factory.h"
#include "gui/Panel.h"
#include "KeyboardHandler.h"
#include "Camera.h"


int FilterSample(int new_sample) {
    static int last_result = 0;
    const int smoothing = 5;
    last_result = (new_sample - last_result) / smoothing + last_result;
    return last_result;
}

int main()
{
    auto& engine = voxie::Engine::GetEngine();
    engine.Init();

    voxie::KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(voxie::FORWARD, engine.GetDeltaTime());
        }});

    voxie::KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(voxie::BACKWARD, engine.GetDeltaTime());
        }});

    voxie::KeyboardHandler::RegisterAction({
            [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(voxie::LEFT, engine.GetDeltaTime());
        }});

    voxie::KeyboardHandler::RegisterAction({
            [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(voxie::RIGHT, engine.GetDeltaTime());
        }});

    voxie::KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
                voxie::MouseHandler::UnlockCamera();
        }});

    voxie::KeyboardHandler::RegisterAction({
          [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                voxie::MouseHandler::LockCamera();
          }});

    auto& models = engine.GetModelHandler();
    auto& lights = engine.GetLightSourceHandler();

    lights.AddLight(MakeLight( "Ambient",
                               voxie::LightType::AMBIENT,
                               {1, 100, 1},
                               {10, 10, 10},
                               glm::vec3{0.1, 0.1, 0.1}));
    lights.AddLight(MakeLight( "Point light 1",
                               voxie::LightType::POINT,
                               {0.5, 1.5, 0.8},
                               {0.05, 0.05, 0.05},
                               glm::vec3{0.8, 0.1, 0.85},
                               {1.0f, 1.5f, 3.8f}));

    lights.AddLight(MakeLight( "Point light 2",
                               voxie::LightType::POINT,
                               {1.5, 0.7, 1.8},
                               {0.05, 0.05, 0.05},
                               glm::vec3{0.1, 0.8, 0.85},
                               {1.0f, 1.5f, 3.8f}));
    engine.onTick.Bind([&] (float deltaTime) {
        gui::ShowSceneOverview();
        gui::ShowSimpleOverlay("FPS: " + std::to_string(FilterSample((int)(1/deltaTime))));

        voxie::helper::RenderingBegin();
        for (auto config : lights.GetRenderingConfigs(engine.GetCamera())) {
            voxie::helper::Submit(config);
        }
        for (auto& config : models.GetRenderingConfigs()) {
            voxie::helper::Submit(config);
        }
        voxie::helper::RenderingEnd();
    });
    engine.StartLoop();
    return 0;
}

