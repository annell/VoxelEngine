#pragma once
#define GL_SILENCE_DEPRECATION

#include "Camera.h"
#include "Core.h"
#include "Delegate.h"
#include "EntityComponentSystem.h"
#include "Lightsource.h"
#include "Logging.h"
#include "MouseHandler.h"
#include "RenderingHandler.h"
#include "Scene.h"
#include "Window.h"
#include <functional>
#include <string>
#include <vector>

class GLFWwindow;

namespace voxie {

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
        [[nodiscard]] std::shared_ptr<Window> GetWindow() const;
        void StartLoop();
        [[nodiscard]] float GetDeltaTime() const;
        EntityComponentSystem &GetComponents();
        RenderingHandler &GetRenderingHandler();
        Scene &GetScene();
        Logging &GetLogger();

        OnTick onTick;

    private:
        void InitGUI() const;

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        Handle camera;
        std::shared_ptr<Window> window;
        std::unique_ptr<EntityComponentSystem> components;
        std::unique_ptr<Scene> scene;
        RenderingHandler renderingHandler;
        Logging logging;

        bool InitWindow();
        void UpdateTime();

        void NewFrame() const;

        void RenderFrame() const;
    };

    namespace helper {
        template<typename T>
        std::shared_ptr<T> GetSceneNode(const Handle &handle) {
            return std::dynamic_pointer_cast<T>(Engine::GetEngine().GetScene().FindNode(handle));
        }
        template<typename T>
        std::shared_ptr<T> GetComponent(const Handle &handle) {
            return Engine::GetEngine().GetComponents().GetComponent<T>(handle);
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
        bool HasComponent(const Handle &handle) {
            return Engine::GetEngine().GetComponents().GetComponent<T>(handle) != nullptr;
        }

        template<typename T>
        void AddComponent(const Handle &handle, std::shared_ptr<T> component) {
            return Engine::GetEngine().GetComponents().AddComponent<T>(handle, component);
        }

        template<typename T>
        void RemoveComponent(const Handle &handle) {
            return Engine::GetEngine().GetComponents().RemoveComponent<T>(handle);
        }

        void RenderingBegin();
        void RenderingEnd();
        void Submit(const RenderingConfig &config);

        void Log(const std::string &log);
    }// namespace helper


}// namespace voxie