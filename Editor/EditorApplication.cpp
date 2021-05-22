#include "Engine.h"

#include <iostream>
#include <memory>
#include "gui/Panel.h"
#include "KeyboardHandler.h"
#include "Camera.h"
#include "TextHandler.h"
#include "Chunk.h"
#include "Cube.h"
#include "ModelLoader.h"
#include "Lightsource.h"

const std::string BASE_PATH = "/Users/stan/dev/C++/VoxelEngine/resources";
const std::string SHADERS = "/shaders";
const std::string FONTS = "/fonts";
const std::string MODELS = "/voxelObjects";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

    voxie::TextHandler text(SCR_WIDTH, SCR_HEIGHT, BASE_PATH + FONTS + "/Arial.ttf",
          std::make_shared<voxie::Shader>(
            std::map<std::string, unsigned int>{
            std::make_pair(BASE_PATH + SHADERS + "/text.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/text.fs", GL_FRAGMENT_SHADER)
            }));
    text.Init();


    voxie::Chunk model("Girl", std::make_shared<voxie::Shader>(
        std::map<std::string, unsigned int>{
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
        }), std::make_shared<voxie::Position>(0, 0, 0));
    voxie::ModelLoader::LoadModel(BASE_PATH + MODELS + "/chr_knight.vox", &model);
    model.Init();
    engine.GetScene().AddEntity(model.GetEntity());

    voxie::Chunk model2("Boy", std::make_shared<voxie::Shader>(
        std::map<std::string, unsigned int>{
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
        }), std::make_shared<voxie::Position>(0, 0, 0));
    voxie::ModelLoader::LoadModel(BASE_PATH + MODELS + "/chr_sword.vox", &model2);
    model2.Init();
    engine.GetScene().AddEntity(model2.GetEntity());

    voxie::Chunk floor("Floor", std::make_shared<voxie::Shader>(
        std::map<std::string, unsigned int>{
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER) 
        }), std::make_shared<voxie::Position>(0, 0, 0));
    floor.AddCube({0, 0, 0}, std::make_unique<voxie::Cube>(
            voxie::Position{0, -0.1, 0},
            voxie::Dimensions{10000, 0.1, 10000},
            voxie::Material{{0.0f, 0.0f, 0.0f},
                     {0.5f, 0.5f, 0.5f},
                     {0.5f, 0.5f, 0.5f},
                     {0.5f, 0.5f, 0.5f},
                     32.0f}, 0));
    floor.Init();
    engine.GetScene().AddEntity(floor.GetEntity());
    auto shaderEntity = voxie::Entity::MakeEntity("shaderEntity");
    voxie::helper::AddComponent(std::make_shared<voxie::Shader>(std::map<std::string, unsigned int>{std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                             std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
                            }), *shaderEntity);

    std::vector<std::shared_ptr<voxie::Shader>> shaders;
    shaders.push_back(model.GetShader());
    shaders.push_back(model2.GetShader());
    shaders.push_back(floor.GetShader());
    voxie::LightSourceHandler lights(shaders);

    lights.AddLight(voxie::LightSource(
        voxie::LightConfig{
            voxie::Entity::MakeEntity("Ambient"),
            std::make_shared<voxie::Shader>(std::map<std::string, unsigned int>{
                    std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                    std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
            }),
            std::make_shared<voxie::Position>(1.0f, 100.0f, 1.0f),
            voxie::LightType::AMBIENT,
            std::make_shared<voxie::Cube>(voxie::Position{0, 0, 0}, voxie::Dimensions{10.0, 10.0, 10.0}),
            std::make_shared<voxie::Color>(glm::vec3{0.15f, 0.15f, 0.15f})
        }));

    lights.AddLight(voxie::LightSource(
        voxie::LightConfig{
            voxie::Entity::MakeEntity("Point Light 1"),
            std::make_shared<voxie::Shader>(std::map<std::string, unsigned int>{
                    std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                    std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
            }),
            std::make_shared<voxie::Position>(0.5f, 1.5f, 0.8f),
            voxie::LightType::POINT,
            std::make_shared<voxie::Cube>(voxie::Position{0, 0, 0}, voxie::Dimensions{0.05, 0.05, 0.05}),
            std::make_shared<voxie::Color>(glm::vec3{0.8f, 0.1f, 0.85f}),
            std::make_shared<voxie::Atteunation>(1.0f, 1.5f, 3.8f)
        }));

    lights.AddLight(voxie::LightSource(
            voxie::LightConfig{
            voxie::Entity::MakeEntity("Point Light 2"),
            std::make_shared<voxie::Shader>(std::map<std::string, unsigned int>{
                    std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                    std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
            }),
            std::make_shared<voxie::Position>(1.5f, 0.7f, 1.5f),
            voxie::LightType::POINT,
            std::make_shared<voxie::Cube>(voxie::Position{0, 0, 0}, voxie::Dimensions{0.05, 0.05, 0.05}),
            std::make_shared<voxie::Color>(glm::vec3{0.1f, 0.8f, 0.85f}),
            std::make_shared<voxie::Atteunation>(1.0f, 1.5f, 3.8f)
        }));

    std::cout << "NrVertex: " << model.NrVertex() << std::endl;


    int n = 0;
    int FPSUpdate = 10;
    std::string fps = "FPS: 0";
    std::string buf;
    engine.onTick.Bind([&] (float deltaTime) {
        n++;
        if (FPSUpdate <= 0) {
            fps = "FPS: " + std::to_string((int)(1/deltaTime));
            FPSUpdate = 10;
        }
        FPSUpdate--;
        std::string log = fps + "\n";

        gui::ShowSceneOverview();
        gui::ShowSimpleOverlay(fps);

        voxie::helper::Begin();
        for (auto config : lights.GetRenderingConfigs(engine.GetCamera())) {
            voxie::helper::Submit(config);
        }
        voxie::helper::Submit(model.GetRenderingConfig());
        voxie::helper::Submit(model2.GetRenderingConfig());
        voxie::helper::Submit(floor.GetRenderingConfig());
        voxie::helper::End();

    });
    engine.StartLoop();
    return 0;
}

