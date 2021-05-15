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

class GLFWwindow;

namespace engine {

namespace rendering {
    class Camera;
}


using OnTick = utility::Delegate<float>;
class Engine {
private:
    Engine() = default;
    ~Engine();

public:
    static Engine& GetEngine();
    bool Init();
    std::shared_ptr<rendering::Camera> GetCamera();
    std::shared_ptr<rendering::Window> GetWindow();
    void StartLoop();
    float GetDeltaTime() const;
    entities::EntityComponentSystem& GetComponents();
    rendering::RenderingHandler& GetRenderingHandler();
    utility::Logging& GetLogger();

    OnTick onTick;
private:
    const unsigned int SCR_WIDTH = 1024;
    const unsigned int SCR_HEIGHT = 768;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    std::shared_ptr<rendering::Window> window;
    std::shared_ptr<rendering::Camera> camera;
    entities::EntityComponentSystem components;
    rendering::RenderingHandler renderingHandler;
    utility::Logging logging;
};

namespace helper {
    template <typename T>
    std::shared_ptr<T> GetComponent(const entities::Entity& handle) {
        return Engine::GetEngine().GetComponents().GetComponent<T>(handle);
    }

    template <typename T>
    void AddComponent(std::shared_ptr<T> component, const entities::Entity& handle) {
        return Engine::GetEngine().GetComponents().AddComponent<T>(component, handle);
    }

    namespace rendering {
        void Begin();
        void End();
        void Submit(const engine::rendering::RenderingConfig& config);
    }

    void Log(std::string log);
}


}
