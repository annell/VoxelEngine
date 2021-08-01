#include "Engine.h"

#include <iostream>
#include <memory>
#include "gui/Panel.h"
#include "KeyboardHandler.h"
#include "Camera.h"
#include "Chunk.h"
#include "Cube.h"
#include "ModelLoader.h"
#include "Lightsource.h"

const std::string BASE_PATH = "/Users/stan/dev/C++/VoxelEngine/resources";
const std::string SHADERS = "/shaders";
const std::string FONTS = "/fonts";
const std::string MODELS = "/voxelObjects";

std::unique_ptr<voxie::Chunk> MakeModel(std::string name, std::string modelPath = "") {
    auto model = std::make_unique<voxie::Chunk>(name, std::make_shared<voxie::Shader>(
            std::map<std::string, unsigned int>{
                    std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
                    std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
            }), std::make_shared<voxie::Position>(0, 0, 0));
    if (!modelPath.empty()) {
        voxie::ModelLoader::LoadModel(BASE_PATH + MODELS + modelPath, model.get());
    }
    model->Init();
    voxie::Engine::GetEngine().GetScene().AddEntity(model->GetEntity());
    return std::move(model);
}

voxie::LightSource MakeLight(std::string name,
                             voxie::LightType type,
                             voxie::Position position = {0, 0, 0},
                             voxie::Dimensions dimensions = {10, 10, 10},
                             voxie::Color color = glm::vec3{0, 0, 0},
                             voxie::Atteunation atteunation = {0, 0, 0}) {
    return voxie::LightSource(
            voxie::LightConfig{
                    voxie::Entity::MakeEntity(name),
                    std::make_shared<voxie::Shader>(std::map<std::string, unsigned int>{
                            std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                            std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
                    }),
                    std::make_shared<voxie::Position>(position),
                    type,
                    std::make_shared<voxie::Cube>(voxie::Position{0, 0, 0}, dimensions),
                    std::make_shared<voxie::Color>(color),
                    std::make_shared<voxie::Atteunation>(atteunation)
            });
}

int FilterSample(int new_sample) {
    static int last_result = 0;
    const int smoothing = 100;
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

    std::vector<std::unique_ptr<voxie::Chunk>> models;
    models.push_back(MakeModel("Girl", "/chr_knight.vox"));
    models.push_back(MakeModel("Boy", "/chr_sword.vox"));
    models.push_back(MakeModel("Floor", "/floor.vox"));

    std::vector<std::shared_ptr<voxie::Shader>> shaders;
    for (auto& model : models) {
        shaders.push_back(model->GetShader());
    }
    voxie::LightSourceHandler lights(shaders);

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
        for (auto& model : models) {
            voxie::helper::Submit(model->GetRenderingConfig());
        }
        voxie::helper::RenderingEnd();

    });
    engine.StartLoop();
    return 0;
}

