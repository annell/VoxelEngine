#pragma once

#include <functional>
#include <vector>
#include "utility/Delegate.h"
#include "entities/EntityComponentSystem.h"
#define GL_SILENCE_DEPRECATION

class Camera;
class GLFWwindow;


using OnTick = Delegate<float>;
class Engine {
private:
    Engine() = default;
    ~Engine();

public:
    static Engine& GetEngine();
    bool Init();
    Camera* GetCamera();
    GLFWwindow* GetWindow();
    void StartLoop();
    float GetDeltaTime() const;
    EntityComponentSystem& GetComponents();

    OnTick onTick;
private:
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    GLFWwindow* window = nullptr;
    Camera* camera = nullptr;
    EntityComponentSystem components;
};
namespace EngineHelper {
    template <typename T>
    std::shared_ptr<T> GetComponent(const Entity& handle) {
        return Engine::GetEngine().GetComponents().GetComponent<T>(handle);
    }

    template <typename T>
    void AddComponent(std::shared_ptr<T> component, const Entity& handle) {
        return Engine::GetEngine().GetComponents().AddComponent<T>(component, handle);
    }
}
