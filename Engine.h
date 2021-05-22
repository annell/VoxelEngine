#pragma once
#define GL_SILENCE_DEPRECATION

#include "Core.h"
#include <functional>
#include <vector>
#include <string>
#include "RenderingHandler.h"
#include "Delegate.h"
#include "EntityComponentSystem.h"
#include "Logging.h"
#include "MouseHandler.h"
#include "Window.h"
#include "Scene.h"

class GLFWwindow;

namespace voxie {

class Camera;


using OnTick = Delegate<float>;
class Engine {
private:
    Engine() = default;
    ~Engine();

public:
    static Engine& GetEngine();
    bool Init();
    std::shared_ptr<Camera> GetCamera();
    std::shared_ptr<Window> GetWindow() const;
    void StartLoop();
    float GetDeltaTime() const;
    EntityComponentSystem& GetComponents();
    RenderingHandler& GetRenderingHandler();
    Scene& GetScene();
    Logging& GetLogger();

    OnTick onTick;
private:
    void InitImGui() const;

    const unsigned int SCR_WIDTH = 1024;
    const unsigned int SCR_HEIGHT = 768;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    std::shared_ptr<Window> window;
    std::shared_ptr<Camera> camera;
    EntityComponentSystem components;
    Scene scene;
    RenderingHandler renderingHandler;
    Logging logging;
};

namespace helper {
    template <typename T>
    std::shared_ptr<T> GetComponent(const Entity& handle) {
        return Engine::GetEngine().GetComponents().GetComponent<T>(handle);
    }

    template <typename T>
    bool HasComponent(const Entity& handle) {
        return Engine::GetEngine().GetComponents().GetComponent<T>(handle) != nullptr;
    }

    template <typename T>
    void AddComponent(std::shared_ptr<T> component, Entity& handle) {
        return Engine::GetEngine().GetComponents().AddComponent<T>(component, handle);
    }

    void Begin();
    void End();
    void Submit(const RenderingConfig& config);

    void Log(std::string log);
}


}
