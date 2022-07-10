#include "RenderingInterface.h"
#include "Core.h"
#include "Window.h"

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

namespace voxie {
    void RenderingInterface::Init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }
    void RenderingInterface::Shutdown() {
        glfwTerminate();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    std::shared_ptr<class Window> RenderingInterface::CreateWindow() {
        auto window = std::make_shared<Window>(glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Voxie", nullptr, nullptr), SCR_WIDTH,
                                               SCR_HEIGHT);
        if (window == nullptr) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return {};
        }
        glfwMakeContextCurrent(window->GetWindow());
        glfwSetFramebufferSizeCallback(window->GetWindow(), MouseHandler::framebuffer_size_callback);
        glfwSetCursorPosCallback(window->GetWindow(), MouseHandler::mouse_movement_callback);
        glfwSetScrollCallback(window->GetWindow(), MouseHandler::scroll_callback);
        glfwSetMouseButtonCallback(window->GetWindow(), MouseHandler::mouse_clicked_callback);
        glfwSwapInterval(1);

        glewInit();

        glEnable(GL_DEPTH_TEST);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
        ImGui_ImplOpenGL3_Init(nullptr);
        ImGui::StyleColorsDark();
        return std::move(window);
    }

    void RenderingInterface::NewFrame() {
        glClearColor(0.25f, 0.6f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void RenderingInterface::RenderFrame(Window *window) {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }

    float RenderingInterface::GetTime() {
        return glfwGetTime();
    }
    bool RenderingInterface::ShouldClose(Window *window) {
        return glfwWindowShouldClose(window->GetWindow());
    }
}// namespace voxie