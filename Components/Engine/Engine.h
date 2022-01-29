#pragma once

#include "RenderingHandler.h"
#include "Scene.h"
#include "Delegate.h"
#include "Logging.h"
#include "TextHandler.h"

class GLFWwindow;

namespace voxie {
    class Camera;
    class Window;

    using OnTick = Delegate<float>;
    class Engine {
    private:
        Engine();
        ~Engine();

    public:
        static Engine &GetEngine();
        bool Init();
        std::shared_ptr<Camera> GetCamera();
        void SetCamera(const Handle &);
        std::shared_ptr<Window> GetWindow() const;
        void StartLoop();
        float GetDeltaTime() const;
        RenderingHandler &GetRenderingHandler();
        Scene &GetScene();
        Logging &GetLogger();
        ECSManager &GetECSManager();

        OnTick onTick;

    private:
        void InitGUI() const;
        void RegisterComponents();
        bool InitWindow();
        void UpdateTime();
        void NewFrame() const;
        void RenderFrame() const;

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        Handle camera;
        std::shared_ptr<Window> window;
        std::unique_ptr<Scene> scene;
        RenderingHandler renderingHandler;
        Logging logging;
        std::unique_ptr<TextHandler> textHandler;
        ECSManager ecsManager;
    };

    namespace helper {
        template<typename T>
        std::shared_ptr<T> GetSceneNode(const Handle &handle) {
            return std::dynamic_pointer_cast<T>(Engine::GetEngine().GetScene().FindNode(handle));
        }

        template<typename T>
        std::vector<std::shared_ptr<T>> GetSceneNodes(Scene::SceneEntities entities) {
            std::vector<std::shared_ptr<T>> nodes;
            for (auto &entity : entities) {
                if (auto node = GetSceneNode<T>(entity)) {
                    nodes.push_back(node);
                }
            }
            return nodes;
        }

        template<typename T>
        std::shared_ptr<T> GetComponent(const Handle &handle) {
            return Engine::GetEngine().GetECSManager().Get<T>(handle);
        }

        template<typename T>
        bool HasComponent(const Handle &handle) {
            return GetComponent<T>(handle) != nullptr;
        }

        template<typename T>
        void AddComponent(const Handle &handle, std::shared_ptr<T> component) {
            return Engine::GetEngine().GetECSManager().Add<T>(handle, std::move(component));
        }

        template<typename T>
        void RemoveComponent(const Handle &handle) {
            return Engine::GetEngine().GetECSManager().Remove<T>(handle);
        }

        void RenderingBegin();
        void RenderingEnd();
        void Submit(const RenderingConfig &config);

        void Log(const std::string &log);
    }// namespace helper


}// namespace voxie
