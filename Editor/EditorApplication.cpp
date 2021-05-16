#include "Engine.h"

#include <iostream>
#include <memory>
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

void ShowEntityPositionController() {
    static int selected = 0;
    ImGui::Begin("Entities controller");
    std::vector<const char*> items;
    auto& entities = engine::Engine::GetEngine().GetScene().GetEntities();
    for (auto& entity : entities) {
        items.push_back(entity->GetName().c_str());
    }
    auto pos = engine::helper::GetComponent<engine::entities::Position>(*entities.at(selected));
    float translation[3] = {pos->x, pos->y, pos->z};
    ImGui::ListBox("Entities", &selected, &items[0], entities.size());
    ImGui::SliderFloat3("(X, Y, Z)", translation, -2.0f, 2.0f);
    auto shader = engine::helper::GetComponent<engine::rendering::Shader>(*entities.at(selected));
    pos->SetPosition(translation[0], translation[1], translation[2]);
    shader->use();
    shader->setMat4("model", pos->model);
    ImGui::End();
}

void ShowExampleAppSimpleOverlay(std::string text, bool* p_open = nullptr)
{
    const float PAD = 10.0f;
    static int corner = 1;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay", p_open, window_flags))
    {
        ImGui::Text(text.c_str());
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (p_open && ImGui::MenuItem("Close")) *p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

int main()
{
    auto& engine = engine::Engine::GetEngine();
    engine.Init();

    engine::input::KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(engine::rendering::FORWARD, engine.GetDeltaTime());
        }});

    engine::input::KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(engine::rendering::BACKWARD, engine.GetDeltaTime());
        }});

    engine::input::KeyboardHandler::RegisterAction({
            [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(engine::rendering::LEFT, engine.GetDeltaTime());
        }});

    engine::input::KeyboardHandler::RegisterAction({
            [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
                engine.GetCamera()->ProcessKeyboard(engine::rendering::RIGHT, engine.GetDeltaTime());
        }});

    engine::input::KeyboardHandler::RegisterAction({
        [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
                engine::input::MouseHandler::UnlockCamera();
        }});

    engine::input::KeyboardHandler::RegisterAction({
          [&engine] () {
            if (glfwGetKey(engine.GetWindow()->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                engine::input::MouseHandler::LockCamera();
          }});

    engine::utility::TextHandler text(SCR_WIDTH, SCR_HEIGHT, BASE_PATH + FONTS + "/Arial.ttf",
          std::make_shared<engine::rendering::Shader>(
            std::map<std::string, unsigned int>{
            std::make_pair(BASE_PATH + SHADERS + "/text.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/text.fs", GL_FRAGMENT_SHADER)
            }));
    text.Init();


    engine::entities::Chunk model("Girl", std::make_shared<engine::rendering::Shader>(
        std::map<std::string, unsigned int>{
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
        }), std::make_shared<engine::entities::Position>(0, 0, 0));
    engine::utility::ModelLoader::LoadModel(BASE_PATH + MODELS + "/chr_knight.vox", &model);
    model.Init();
    engine.GetScene().AddEntity(model.GetEntity());

    engine::entities::Chunk model2("Boy", std::make_shared<engine::rendering::Shader>(
        std::map<std::string, unsigned int>{
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
        }), std::make_shared<engine::entities::Position>(0, 0, 0));
    engine::utility::ModelLoader::LoadModel(BASE_PATH + MODELS + "/chr_sword.vox", &model2);
    model2.Init();
    engine.GetScene().AddEntity(model2.GetEntity());

    engine::entities::Chunk floor("Floor", std::make_shared<engine::rendering::Shader>(
        std::map<std::string, unsigned int>{
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
            std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER) 
        }), std::make_shared<engine::entities::Position>(0, 0, 0));
    floor.AddCube(std::make_unique<engine::entities::Cube>(
            engine::entities::Position{0, -0.1, 0},
            engine::entities::Dimensions{10000, 0.1, 10000},
            engine::entities::Material{{0.0f, 0.0f, 0.0f},
                     {0.5f, 0.5f, 0.5f},
                     {0.5f, 0.5f, 0.5f},
                     {0.5f, 0.5f, 0.5f},
                     32.0f}, 0));
    floor.Init();
    engine.GetScene().AddEntity(floor.GetEntity());
    auto shaderEntity = engine::entities::Entity::MakeEntity("shaderEntity");
    engine::helper::AddComponent(std::make_shared<engine::rendering::Shader>(std::map<std::string, unsigned int>{std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                             std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
                            }), *shaderEntity);

    std::vector<std::shared_ptr<engine::rendering::Shader>> shaders;
    shaders.push_back(model.GetShader());
    shaders.push_back(model2.GetShader());
    shaders.push_back(floor.GetShader());
    engine::entities::LightSourceHandler lights(std::make_shared<engine::rendering::Shader>(std::map<std::string, unsigned int>{
        std::make_pair(BASE_PATH + SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
        std::make_pair(BASE_PATH + SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
    }), shaders);

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
    std::string buf;
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
        std::string log = fps + "\n";

        ShowEntityPositionController();
        ShowExampleAppSimpleOverlay(fps);

        engine::helper::rendering::Begin();
        engine::helper::rendering::Submit(model.GetRenderingConfig());
        engine::helper::rendering::Submit(model2.GetRenderingConfig());
        engine::helper::rendering::Submit(floor.GetRenderingConfig());
        for (auto config : lights.GetRenderingConfigs(engine.GetCamera())) {
            engine::helper::rendering::Submit(config);
        }
        engine::helper::rendering::End();

    });
    engine.StartLoop();
    return 0;
}

