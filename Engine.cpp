#include "Engine.h"
#include <iostream>
#include <TextHandler.h>
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "Camera.h"

namespace voxie {

Engine::~Engine() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool Engine::Init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = std::make_shared<Window>(glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Voxie", NULL, NULL), SCR_WIDTH, SCR_HEIGHT);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window->GetWindow());
    glfwSetFramebufferSizeCallback(window->GetWindow(), MouseHandler::framebuffer_size_callback);
    glfwSetCursorPosCallback(window->GetWindow(), MouseHandler::mouse_callback);
    glfwSetScrollCallback(window->GetWindow(), MouseHandler::scroll_callback);
    glfwSwapInterval(0);

    glewInit();

    glEnable(GL_DEPTH_TEST);

    camera = std::make_shared<Camera>(Entity::MakeEntity("Editor camera"), glm::vec3(-3.0f, 1.0f, -3.0f));
    GetScene().AddEntity(camera->GetEntity());

    InitImGui();

    return true;
}

std::shared_ptr<Camera> Engine::GetCamera() {
    return camera;
}

std::shared_ptr<Window> Engine::GetWindow() const {
    return window;
}

void Engine::StartLoop() {
    while (!glfwWindowShouldClose(window->GetWindow())) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.25f, 0.6f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        KeyboardHandler::processInput();

        onTick.Broadcast(GetDeltaTime());
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }

    glfwTerminate();
}

float Engine::GetDeltaTime() const {
    return deltaTime;
}

Engine &Engine::GetEngine() {
    static Engine engine;
    return engine;
}

EntityComponentSystem &Engine::GetComponents() {
    return components;
}

RenderingHandler &Engine::GetRenderingHandler() {
    return renderingHandler;
}

Logging &Engine::GetLogger() {
    return logging;
}

Scene &Engine::GetScene() {
    return scene;
}

void Engine::InitImGui() const {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(GetWindow()->GetWindow(), true);
    ImGui_ImplOpenGL3_Init(nullptr);
    ImGui::StyleColorsDark();
}

    namespace helper {

void Log(std::string log) {
    Engine::GetEngine().GetLogger().AddLog(log.c_str());
}

void Begin() {
    Engine::GetEngine().GetRenderingHandler().Begin(Engine::GetEngine().GetCamera());
}

void End() {
    Engine::GetEngine().GetRenderingHandler().End();
}

void Submit(const voxie::RenderingConfig& config) {
    Engine::GetEngine().GetRenderingHandler().Submit(config);
}
}

}
