#include "Engine.h"
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>
#include "MouseHandler.h"
#include "KeyboardHandler.h"
#include "Camera.h"


Engine::~Engine() {
    if (camera) {
        delete camera;
    }
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
    glfwSetFramebufferSizeCallback(window, MouseHandler::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, MouseHandler::mouse_callback);
    glfwSetScrollCallback(window, MouseHandler::scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewInit();

    glEnable(GL_DEPTH_TEST);

    camera = new Camera(glm::vec3(0.0f, 1.0f, 3.0f));
    return true;
}

Camera *Engine::GetCamera() {
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

        KeyboardHandler::processInput();
        for (auto callback : OnTickCallbacks) {
            callback(deltaTime);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Engine::RegisterOnTick(Engine::OnTick callback) {
    OnTickCallbacks.push_back(callback);
}

float Engine::GetDeltaTime() const {
    return deltaTime;
}

Engine &Engine::GetEngine() {
    static Engine engine;
    return engine;
}
