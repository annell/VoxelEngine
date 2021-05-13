#pragma once

#include <functional>
#include <vector>
#include "Delegate.h"
#include "EntityComponentSystem.h"
#define GL_SILENCE_DEPRECATION

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
    rendering::Camera* GetCamera();
    GLFWwindow* GetWindow();
    void StartLoop();
    float GetDeltaTime() const;
    entities::EntityComponentSystem& GetComponents();

    OnTick onTick;
private:
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    GLFWwindow* window = nullptr;
    rendering::Camera* camera = nullptr;
    entities::EntityComponentSystem components;
};
namespace EngineHelper {
    template <typename T>
    std::shared_ptr<T> GetComponent(const entities::Entity& handle) {
        return Engine::GetEngine().GetComponents().GetComponent<T>(handle);
    }

    template <typename T>
    void AddComponent(std::shared_ptr<T> component, const entities::Entity& handle) {
        return Engine::GetEngine().GetComponents().AddComponent<T>(component, handle);
    }
}

}
