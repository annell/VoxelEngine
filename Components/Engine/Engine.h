#pragma once

#include "Delegate.h"
#include "PhysicsHandler.h"
#include "RenderingHandler.h"
#include "Scene.h"
#include "TextHandler.h"

class GLFWwindow;

namespace voxie {
    class Camera;
    class Window;
    class GameMode;
    class Text;

    using OnTick = Delegate<float>;
    class Engine {
    private:
        Engine();
        ~Engine();

    public:
        static Engine &GetEngine();

        void SetGameMode(std::unique_ptr<GameMode>);
        void SetCamera(const Handle &);

        std::shared_ptr<Camera> GetCamera();
        Window *GetWindow() const;
        Scene *GetScene() const;
        ECSManager *GetECSManager();
        GameMode *GetGameMode() const;
        PhysicsHandler *GetPhysicsHandler();
        RenderingHandler *GetRenderingHandler();

        bool IsRunning() const;
        void StartLoop();
        void StopLoop();
        float GetDeltaTime() const;
        OnTick onTick;

    private:
        bool Init();
        void RegisterComponents();
        bool InitWindow();
        void UpdateTime();
        void SubmitNodesForRendering(const Scene::SceneNodes &) const;
        void SubmitTextForRendering(const std::vector<std::shared_ptr<voxie::Text>> &) const;

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        bool isRunning = false;
        Handle camera;

        std::shared_ptr<Window> window;
        std::unique_ptr<Scene> scene;
        std::unique_ptr<GameMode> gameMode;
        std::unique_ptr<TextHandler> textHandler;

        RenderingHandler renderingHandler;
        ECSManager ecsManager;
        PhysicsHandler physicsHandler;
    };

    namespace helper {
        template<typename T>
        std::shared_ptr<T> GetSceneNode(const Handle &handle) {
            return std::dynamic_pointer_cast<T>(Engine::GetEngine().GetScene()->FindNode(handle));
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
            return Engine::GetEngine().GetECSManager()->Get<T>(handle);
        }

        template<typename T>
        bool HasComponent(const Handle &handle) {
            return Engine::GetEngine().GetECSManager()->Has<T>(handle);
        }

        template<typename T>
        void AddComponent(const Handle &handle, std::shared_ptr<T> component) {
            return Engine::GetEngine().GetECSManager()->Add<T>(handle, std::move(component));
        }

        template<typename T>
        void RemoveComponent(const Handle &handle) {
            return Engine::GetEngine().GetECSManager()->Remove<T>(handle);
        }

        void RenderingBegin();
        void RenderingEnd();
        void Submit(const RenderingConfig &config);
    }// namespace helper


}// namespace voxie
