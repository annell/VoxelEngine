#include "Engine.h"
#include <iostream>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "Camera.h"

namespace engine {

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

    window = std::make_shared<rendering::Window>(glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "VoxelEngine", NULL, NULL), SCR_WIDTH, SCR_HEIGHT);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window->GetWindow());
    glfwSetFramebufferSizeCallback(window->GetWindow(), input::MouseHandler::framebuffer_size_callback);
    glfwSetCursorPosCallback(window->GetWindow(), input::MouseHandler::mouse_callback);
    glfwSetScrollCallback(window->GetWindow(), input::MouseHandler::scroll_callback);

    glewInit();

    glEnable(GL_DEPTH_TEST);

    camera = std::make_shared<rendering::Camera>(glm::vec3(0.0f, 1.0f, 3.0f));
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(GetWindow()->GetWindow(), true);
    ImGui_ImplOpenGL3_Init(nullptr);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    return true;
}

std::shared_ptr<rendering::Camera> Engine::GetCamera() {
    return camera;
}

std::shared_ptr<rendering::Window> Engine::GetWindow() {
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

        input::KeyboardHandler::processInput();
        onTick.Broadcast(GetDeltaTime());
        ImGui::SetNextWindowSize({(float)SCR_WIDTH, (float)SCR_HEIGHT / 4});
        ImGui::SetNextWindowPos({0, (float)SCR_HEIGHT - (float)SCR_HEIGHT / 4});
        logging.Draw("Logger", nullptr);
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

entities::EntityComponentSystem &Engine::GetComponents() {
    return components;
}

rendering::RenderingHandler &Engine::GetRenderingHandler() {
    return renderingHandler;
}

utility::Logging &Engine::GetLogger() {
    return logging;
}

namespace helper {
void Log(std::string log) {
    Engine::GetEngine().GetLogger().AddLog(log.c_str());
}
}

namespace helper::rendering {

void Begin() {
    Engine::GetEngine().GetRenderingHandler().Begin(Engine::GetEngine().GetCamera());
}

void End() {
    Engine::GetEngine().GetRenderingHandler().End();
}

void Submit(const engine::rendering::RenderingConfig& config) {
    Engine::GetEngine().GetRenderingHandler().Submit(config);
}
}

}
