#include "Engine.h"
#include <iostream>
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
    if (!InitWindow()) {
        return false;
    }
    InitCamera();
    InitGUI();

    return true;
}

void Engine::InitCamera() {
    camera = std::__1::make_shared<Camera>(Entity::MakeEntity(), Name("Editor Camera"), glm::vec3(-3.0f, 1.0f, -3.0f));
    GetScene().AddEntity(camera->GetEntity());
}

bool Engine::InitWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    const unsigned int SCR_WIDTH = 1024;
    const unsigned int SCR_HEIGHT = 768;
    window = std::__1::make_shared<Window>(glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Voxie", NULL, NULL), SCR_WIDTH,
                                           SCR_HEIGHT);
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
    //glfwSwapInterval(0);

    glewInit();

    glEnable(GL_DEPTH_TEST);
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
        UpdateTime();

        NewFrame();

        KeyboardHandler::processInput();
        onTick.Broadcast(GetDeltaTime());

        RenderFrame();
    }

    glfwTerminate();
}

void Engine::RenderFrame() const {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window->GetWindow());
    glfwPollEvents();
}

void Engine::NewFrame() const {
    glClearColor(0.25f, 0.6f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Engine::UpdateTime() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
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

void Engine::InitGUI() const {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplGlfw_InitForOpenGL(GetWindow()->GetWindow(), true);
    ImGui_ImplOpenGL3_Init(nullptr);
    ImGui::StyleColorsDark();
}

namespace helper {

void Log(std::string log) {
    Engine::GetEngine().GetLogger().AddLog(log.c_str());
}

void RenderingBegin() {
    Engine::GetEngine().GetRenderingHandler().Begin(Engine::GetEngine().GetCamera());
}

void RenderingEnd() {
    Engine::GetEngine().GetRenderingHandler().End();
}

void Submit(const voxie::RenderingConfig& config) {
    Engine::GetEngine().GetRenderingHandler().Submit(config);
}
}

}
