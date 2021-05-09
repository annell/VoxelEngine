#pragma once

#include <functional>
#include <vector>

class Camera;
class GLFWwindow;

class Engine {
public:
    Engine() = default;
    ~Engine();

    static Engine& GetEngine();
    bool Init();
    Camera* GetCamera();
    GLFWwindow* GetWindow();
    void StartLoop();
    float GetDeltaTime() const;

    using OnTick = std::function<void(float)>;
    void RegisterOnTick(OnTick);
private:
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    GLFWwindow* window = nullptr;
    Camera* camera = nullptr;
    std::vector<OnTick> OnTickCallbacks;
};
