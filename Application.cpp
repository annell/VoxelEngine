#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "rendering/Shader.h"
#include "rendering/Camera.h"
#include "entities/Cube.h"
#include "entities/Lightsource.h"
#include "entities/Chunk.h"
#include "utility/ModelLoader.h"
#include "utility/TextHandler.h"
#include "Engine.h"
#include "input/KeyboardHandler.h"
#include "input/MouseHandler.h"

#include <iostream>
#include <memory>

const std::string BASE_PATH = "/Users/stan/dev/C++/VoxelEngine/resources";
const std::string SHADERS = "/shaders";
const std::string FONTS = "/fonts";
const std::string MODELS = "/voxelObjects";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    auto& engine = engine::Engine::GetEngine();
    engine.Init();

    engine::input::KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(engine.GetWindow(), true);
        }});

    engine::input::KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(engine::rendering::FORWARD, engine.GetDeltaTime());
        }});

    engine::input::KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(engine::rendering::BACKWARD, engine.GetDeltaTime());
        }});

    engine::input::KeyboardHandler::RegisterAction({
            [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(engine::rendering::LEFT, engine.GetDeltaTime());
        }});

    engine::input::KeyboardHandler::RegisterAction({
            [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(engine::rendering::RIGHT, engine.GetDeltaTime());
        }});

    engine::input::KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                engine::input::MouseHandler::UnlockCamera();
        }});

    engine::input::KeyboardHandler::RegisterAction({
          [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
                engine::input::MouseHandler::LockCamera();
          }});

    engine::utility::TextHandler text(SCR_WIDTH, SCR_HEIGHT, BASE_PATH + FONTS + "/Arial.ttf",
          engine::rendering::Shader(
            {
            std::make_pair(BASE_PATH + SHADERS + "/text.vs", GL_VERTEX_SHADER), 
            std::make_pair(BASE_PATH + SHADERS + "/text.fs", GL_FRAGMENT_SHADER)
            }));
    text.Init();

    engine::rendering::Shader lightCubeShader(
        {
        std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
        std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
        });

    engine::entities::Chunk model(engine::rendering::Shader(
        {
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
        }), {0, 0, 0});
    engine::utility::ModelLoader::LoadModel(BASE_PATH + MODELS + "/chr_knight.vox", model);
    model.Init();

    engine::entities::Chunk model2(engine::rendering::Shader(
        {
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
        }), {0, 0, 0});
    engine::utility::ModelLoader::LoadModel(BASE_PATH + MODELS + "/chr_sword.vox", model2);
    model2.Init();

    engine::entities::Chunk floor(engine::rendering::Shader(
        {
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER) 
        }), {0, 0, 0});
    floor.AddCube(std::make_unique<engine::entities::Cube>(
            engine::entities::Position{0, -0.1, 0},
            engine::entities::Dimensions{10000, 0.1, 10000},
            engine::entities::Material{{0.0f, 0.0f, 0.0f},
                     {0.5f, 0.5f, 0.5f},
                     {0.5f, 0.5f, 0.5f},
                     {0.5f, 0.5f, 0.5f},
                     32.0f}, 0));
    floor.Init();
    engine::entities::Entity shaderEntity(5);
    engine::EngineHelper::AddComponent(std::make_shared<engine::rendering::Shader>(std::map<std::string, unsigned int>{std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                             std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
                            }), shaderEntity);

    auto component = engine::EngineHelper::GetComponent<engine::rendering::Shader>(shaderEntity);
    auto component2 = engine::EngineHelper::GetComponent<engine::entities::Cube>(shaderEntity);

    std::vector<engine::rendering::Shader*> shaders;
    shaders.push_back(model.GetShader());
    shaders.push_back(model2.GetShader());
    shaders.push_back(floor.GetShader());
    engine::entities::LightSourceHandler lights(&lightCubeShader, shaders);

    lights.AddLight(engine::entities::LightSource(
        engine::entities::LightConfig{
            engine::entities::LightType::AMBIENT,
            new engine::entities::Cube({1, 1, 1}, {100.0, 100.0, 100.0}),
            {0.15f, 0.15f, 0.15f}, 
            {1.0f, 100.0f, 1.0f}
        }));
                                
    lights.AddLight(engine::entities::LightSource(
        engine::entities::LightConfig{
            engine::entities::LightType::POINT,
            new engine::entities::Cube({1, 1, 1}, {0.5, 0.5, 0.5}),
            {0.8f, 0.1f, 0.85f}, 
            {0.5f, 0.5f, 0.5f},
            {1.0f, 1.5f, 3.8f},
        }));

    lights.AddLight(engine::entities::LightSource(
            engine::entities::LightConfig{
            engine::entities::LightType::POINT,
            new engine::entities::Cube({1, 1, 1}, {0.5, 0.5, 0.5}),
            {0.1f, 0.8f, 0.85f}, 
            {1.5f, 0.7f, 1.5f},
            {1.0f, 1.5f, 3.8f},
        }));

    std::cout << "NrVertex: " << model.NrVertex() << std::endl;

    int n = 0;
    int FPSUpdate = 10;
    std::string fps = "FPS: 0";
    engine.onTick.Bind([&] (float deltaTime) {
        n++;
        if (FPSUpdate <= 0) {
            fps = "FPS: " + std::to_string((int)(1/deltaTime));
            FPSUpdate = 10;
        }
        FPSUpdate--;
        for (auto& light : lights.GetLightSources()) {
            auto pos = light.GetPosition();
            light.SetPosition(pos);
        }
        auto pos = model.GetPosition();
        pos.x = pos.x + 0.01;
        model.SetPosition(pos);

        lights.Draw(*engine.GetCamera());
        model.Draw();
        model2.Draw();
        floor.Draw();

        text.RenderText(fps, 15.0f, SCR_HEIGHT - 25.0f, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
    });
    engine.StartLoop();
    return 0;
}

