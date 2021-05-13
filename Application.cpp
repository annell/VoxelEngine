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
    auto& engine = Engine::GetEngine();
    engine.Init();

    KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(engine.GetWindow(), true);
        }});

    KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(FORWARD, engine.GetDeltaTime());
        }});

    KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(BACKWARD, engine.GetDeltaTime());
        }});

    KeyboardHandler::RegisterAction({
            [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(LEFT, engine.GetDeltaTime());
        }});

    KeyboardHandler::RegisterAction({
            [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(RIGHT, engine.GetDeltaTime());
        }});

    KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                MouseHandler::UnlockCamera();
        }});

    KeyboardHandler::RegisterAction({
          [&engine] () {
            if (glfwGetKey(engine.GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
                MouseHandler::LockCamera();
          }});

    TextHandler text(SCR_WIDTH, SCR_HEIGHT, BASE_PATH + FONTS + "/Arial.ttf", 
        Shader(
            {
            std::make_pair(BASE_PATH + SHADERS + "/text.vs", GL_VERTEX_SHADER), 
            std::make_pair(BASE_PATH + SHADERS + "/text.fs", GL_FRAGMENT_SHADER)
            }));
    text.Init();

    Shader lightCubeShader(
        {
        std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
        std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
        });

    Chunk model(Shader(
        {
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
        }), {0, 0, 0});
    ModelLoader::LoadModel(BASE_PATH + MODELS + "/chr_knight.vox", model);
    model.Init();

    Chunk model2(Shader(
        {
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
        }), {0, 0, 0});
    ModelLoader::LoadModel(BASE_PATH + MODELS + "/chr_sword.vox", model2);
    model2.Init();

    Chunk floor(Shader(
        {
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER) 
        }), {0, 0, 0});
    floor.AddCube(std::make_unique<Cube>(
            Position{0, -0.1, 0},
            Dimensions{10000, 0.1, 10000},
            Material{{0.0f, 0.0f, 0.0f},
                     {0.5f, 0.5f, 0.5f},
                     {0.5f, 0.5f, 0.5f},
                     {0.5f, 0.5f, 0.5f},
                     32.0f}, 0));
    floor.Init();
    Entity shaderEntity(5);
    EngineHelper::AddComponent(std::make_shared<Shader>(std::map<std::string, unsigned int>{std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                             std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
                            }), shaderEntity);

    auto component = EngineHelper::GetComponent<Shader>(shaderEntity);
    auto component2 = EngineHelper::GetComponent<Cube>(shaderEntity);

    std::vector<Shader*> shaders;
    shaders.push_back(model.GetShader());
    shaders.push_back(model2.GetShader());
    shaders.push_back(floor.GetShader());
    LightSourceHandler lights(&lightCubeShader, shaders);

    lights.AddLight(LightSource(
        LightConfig{
            LightType::AMBIENT,
            new Cube({1, 1, 1}, {100.0, 100.0, 100.0}), 
            {0.15f, 0.15f, 0.15f}, 
            {1.0f, 100.0f, 1.0f}
        }));
                                
    lights.AddLight(LightSource(
        LightConfig{
            LightType::POINT,
            new Cube({1, 1, 1}, {0.5, 0.5, 0.5}), 
            {0.8f, 0.1f, 0.85f}, 
            {0.5f, 0.5f, 0.5f},
            {1.0f, 1.5f, 3.8f},
        }));

    lights.AddLight(LightSource(
        LightConfig{
            LightType::POINT,
            new Cube({1, 1, 1}, {0.5, 0.5, 0.5}), 
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

