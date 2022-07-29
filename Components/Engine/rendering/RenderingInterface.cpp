#include "RenderingInterface.h"
#include "Core.h"
#include "Window.h"

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

namespace {
    int RenderingTypeToOpenGL(voxie::RenderingType type) {
        using rendering = voxie::RenderingType;
        switch (type) {
            case rendering::Texture0:
                return GL_TEXTURE0;
            case rendering::TextureCubeMap:
                return GL_TEXTURE_CUBE_MAP;
            case rendering::TextureType2D:
                return GL_TEXTURE_2D;
            case rendering::Triangles:
                return GL_TRIANGLES;
            case rendering::TextureMinFilter:
                return GL_TEXTURE_MIN_FILTER;
            case rendering::TextureMagFilter:
                return GL_TEXTURE_MAG_FILTER;
            case rendering::TextureWrapS:
                return GL_TEXTURE_WRAP_S;
            case rendering::TextureWrapT:
                return GL_TEXTURE_WRAP_T;
            case rendering::TextureWrapR:
                return GL_TEXTURE_WRAP_R;
            case rendering::ClampToEdge:
                return GL_CLAMP_TO_EDGE;
            case rendering::TextureCubeMapPositiveX:
                return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            case rendering::TextureCubeMapNegativeX:
                return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
            case rendering::TextureCubeMapPositiveY:
                return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
            case rendering::TextureCubeMapNegativeY:
                return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
            case rendering::TextureCubeMapPositiveZ:
                return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
            case rendering::TextureCubeMapNegativeZ:
                return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
            case rendering::RGB:
                return GL_RGB;
            case rendering::UnsignedByte:
                return GL_UNSIGNED_BYTE;
            case rendering::Less:
                return GL_LESS;
            case rendering::LessEqual:
                return GL_LEQUAL;
            case rendering::Linear:
                return GL_LINEAR;
        }
        return -1;
    }

}// namespace

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
    void RenderingInterface::Draw(std::shared_ptr<VertexBufferArray> vba) {
        glBindVertexArray(vba->VAO);
        glDrawArrays(GL_TRIANGLES, 0, vba->nrVertex);
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

    void RenderingInterface::GenerateTexture(int count, unsigned int *textureID) {
        glGenTextures(count, textureID);
    }

    void RenderingInterface::DepthFunction(RenderingType type) {
        glDepthFunc(RenderingTypeToOpenGL(type));
    }
    void RenderingInterface::ActivateTexture(RenderingType type) {
        glActiveTexture(RenderingTypeToOpenGL(type));
    }

    void RenderingInterface::BindTexture(RenderingType type, unsigned int textureID) {
        glBindTexture(RenderingTypeToOpenGL(type), textureID);
    }
    void RenderingInterface::TextureParameter(RenderingType target, RenderingType paramName, RenderingType param) {
        glTexParameteri(RenderingTypeToOpenGL(target), RenderingTypeToOpenGL(paramName), RenderingTypeToOpenGL(param));
    }
    void RenderingInterface::TextureImage2D(RenderingType target, int level, RenderingType internalFormat, int width, int height, int border, RenderingType format, RenderingType type, const void *pixels) {
        glTexImage2D(RenderingTypeToOpenGL(target), level, RenderingTypeToOpenGL(internalFormat), width, height, border, RenderingTypeToOpenGL(format), RenderingTypeToOpenGL(type), pixels);
    }
}// namespace voxie