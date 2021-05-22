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

void ShowEntityColorController(const voxie::Entity& entity) {
    auto color = voxie::helper::GetComponent<voxie::Color>(entity);
    float updatedColor[3] = {color->color[0], color->color[1], color->color[2]};
    ImGui::ColorPicker3("Color", updatedColor);
    color->SetColor(updatedColor[0], updatedColor[1], updatedColor[2]);
}

void ShowEntityPositionController(const voxie::Entity& entity) {
    auto pos = voxie::helper::GetComponent<voxie::Position>(entity);
    float translation[3] = {pos->x, pos->y, pos->z};
    ImGui::InputFloat3("Position", translation);
    pos->SetPosition(translation[0], translation[1], translation[2]);
    if (auto shader = voxie::helper::GetComponent<voxie::Shader>(entity)) {
        shader->use();
        shader->setMat4("model", pos->model);
    }
}

void ShowSceneOverview() {
    static int selected = 0;
    ImGui::Begin("Controllers");
    std::vector<const char*> items;
    auto& entities = voxie::Engine::GetEngine().GetScene().GetEntities();
    for (auto& entity : entities) {
        items.push_back(entity->GetName().c_str());
    }
    ImGui::ListBox("", &selected, &items[0], entities.size());

    auto& entity = *entities.at(selected);
    if (voxie::helper::HasComponent<voxie::Position>(entity)) {
        ShowEntityPositionController(entity);
    }
    if (voxie::helper::HasComponent<voxie::Color>(entity)) {
        ShowEntityColorController(entity);
    }

    ImGui::End();
}

void ShowSimpleOverlay(std::string text, bool* p_open = nullptr)
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
            {1.0f, 1.5f, 3.8f}
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
        std::string log = fps + "\n";

        ShowSceneOverview();
        ShowSimpleOverlay(fps);

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

