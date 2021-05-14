#include "Engine.h"
#include <iostream>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "Camera.h"

namespace engine {

Engine::~Engine() {
}

bool Engine::Init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "VoxelEngine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, input::MouseHandler::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, input::MouseHandler::mouse_callback);
    glfwSetScrollCallback(window, input::MouseHandler::scroll_callback);

    glewInit();

    glEnable(GL_DEPTH_TEST);

    camera = std::make_shared<rendering::Camera>(glm::vec3(0.0f, 1.0f, 3.0f));
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(GetWindow(), true);
    ImGui_ImplOpenGL3_Init(nullptr);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    return true;
}

std::shared_ptr<rendering::Camera> Engine::GetCamera() {
    return camera;
}

GLFWwindow *Engine::GetWindow() {
    return window;
}

void Engine::StartLoop() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.25f, 0.6f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        input::KeyboardHandler::processInput();
        onTick.Broadcast(GetDeltaTime());

        glfwSwapBuffers(window);
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
