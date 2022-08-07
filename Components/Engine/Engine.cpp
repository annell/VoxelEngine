#include "Engine.h"
#include "Core.h"
#include <iostream>

#include "CubeEntity.h"
#include "GameMode.h"
#include "Outline.h"
#include "RenderingInterface.h"
#include "Text.h"
#include "TimeMeasurement.h"
#include <RigidBody.h>

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

namespace voxie {

    Engine::Engine()
        : scene(std::make_unique<Scene>(BASE_PATH + SCENES + "/")), camera(NullEntity), gameMode(nullptr), textHandler(nullptr) {
    }

    Engine::~Engine() {
        RenderingInterface::Shutdown();
        scene.reset();
    }

    void Engine::RegisterComponents() {
        ecsManager.RegisterComponent<Name>();
        ecsManager.RegisterComponent<Material>();
        ecsManager.RegisterComponent<Color>();
        ecsManager.RegisterComponent<Attenuation>();
        ecsManager.RegisterComponent<Position>();
        ecsManager.RegisterComponent<Position2D>();
        ecsManager.RegisterComponent<Direction>();
        ecsManager.RegisterComponent<Skybox>();
        ecsManager.RegisterComponent<Cube>();
        ecsManager.RegisterComponent<VisibleText>();
        ecsManager.RegisterComponent<Shader>();
        ecsManager.RegisterComponent<RigidBody>();
        ecsManager.RegisterComponent<VertexBufferArray>();
        ecsManager.RegisterComponent<Outline>();
    }

    bool Engine::Init() {
        RegisterComponents();
        if (!InitWindow()) {
            return false;
        }
        GetPhysicsHandler()->Initialize();
        return true;
    }

    bool Engine::InitWindow() {
        RenderingInterface::Init();
        window = RenderingInterface::CreateWindow();

        textHandler = std::make_unique<TextHandler>(SCR_WIDTH, SCR_HEIGHT, BASE_PATH + FONTS + "/Arial.ttf", std::make_shared<voxie::Shader>(std::map<std::string, ShaderType>{std::make_pair(BASE_PATH + SHADERS + "/text.vs", ShaderType::VERTEX), std::make_pair(BASE_PATH + SHADERS + "/text.fs", ShaderType::FRAGMENT)}));
        textHandler->Init();
        return true;
    }

    void Engine::SetGameMode(std::unique_ptr<GameMode> gameModeIn) {
        gameMode = std::move(gameModeIn);
        gameMode->Reset();
    }

    std::shared_ptr<Camera> Engine::GetCamera() {
        if (gameMode && gameMode->IsStarted()) {
            if (auto entity = GetScene()->FindEntity<PlayerController>()) {
                if (auto playerCamera = entity->GetCamera()) {
                    return playerCamera;
                }
            }
        }
        return helper::GetSceneNode<Camera>(camera);
    }

    Window *Engine::GetWindow() const {
        return window.get();
    }

    void Engine::StartLoop() {
        isRunning = true;

        while (IsRunning()) {
            UpdateTime();
            RenderingInterface::NewFrame();
            TimeMeasurment::RenderResults();
            TimeMeasurment::Start("Main");

            TimeMeasurment::Start("processInput");
            KeyboardHandler::processInput();
            TimeMeasurment::End("processInput");

            TimeMeasurment::Start("RenderingBegin");
            voxie::helper::RenderingBegin();
            TimeMeasurment::End("RenderingBegin");

            TimeMeasurment::Start("onTick");
            onTick.Broadcast(GetDeltaTime());
            TimeMeasurment::End("onTick");

            TimeMeasurment::Start("SubmitNodesForRendering");
            SubmitNodesForRendering(GetScene()->GetNodesForRendering());
            TimeMeasurment::End("SubmitNodesForRendering");

            TimeMeasurment::Start("RenderFrame");
            RenderingInterface::RenderFrame(GetWindow());
            TimeMeasurment::End("RenderFrame");

            TimeMeasurment::End("Main");
        }
    }

    void Engine::StopLoop() {
        isRunning = false;
    }

    bool Engine::IsRunning() const {
        return isRunning && !RenderingInterface::ShouldClose(GetWindow());
    }

    void Engine::SubmitNodesForRendering(const Scene::SceneNodes &nodes) const {
        std::vector<std::shared_ptr<voxie::Text>> texts;
        for (const auto &node : nodes) {
            if (auto entity = std::dynamic_pointer_cast<voxie::Chunk>(node)) {
                voxie::helper::Submit(entity->GetRenderingConfig());
            } else if (auto entity = std::dynamic_pointer_cast<voxie::Sprite>(node)) {
                voxie::helper::Submit(entity->GetRenderingConfig());
            } else if (auto entity = std::dynamic_pointer_cast<voxie::CubeEntity>(node)) {
                voxie::helper::Submit(entity->GetRenderingConfig());
            } else if (auto entity = std::dynamic_pointer_cast<voxie::Text>(node)) {
                texts.push_back(entity);
            }
        }

        auto skybox = GetScene()->GetSkybox();
        voxie::helper::Submit(skybox->GetRenderingConfig());

        voxie::helper::RenderingEnd();

        SubmitTextForRendering(texts);
    }

    void Engine::SubmitTextForRendering(const std::vector<std::shared_ptr<voxie::Text>> &texts) const {
        for (auto model : texts) {
            auto pos = model->GetPosition2D();
            textHandler->RenderText(model->GetVisibleText()->text, pos->pos.x, pos->pos.y, 1, model->GetColor()->color);
        }
    }

    void Engine::UpdateTime() {
        float currentFrame = RenderingInterface::GetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    float Engine::GetDeltaTime() const {
        return deltaTime;
    }

    Engine &Engine::GetEngine() {
        static Engine engine;
        static bool initialized = false;
        if (!initialized) {
            initialized = true;
            engine.Init();
        }
        return engine;
    }

    RenderingHandler *Engine::GetRenderingHandler() {
        return &renderingHandler;
    }

    Scene *Engine::GetScene() const {
        return scene.get();
    }

    void Engine::SetCamera(const Handle &handle) {
        camera = handle;
    }
    ECSManager *Engine::GetECSManager() {
        return &ecsManager;
    }

    GameMode *Engine::GetGameMode() const {
        return gameMode.get();
    }
    PhysicsHandler *Engine::GetPhysicsHandler() {
        return &physicsHandler;
    }

    namespace helper {
        void RenderingBegin() {
            Engine::GetEngine().GetRenderingHandler()->Begin();
        }

        void RenderingEnd() {
            Engine::GetEngine().GetRenderingHandler()->End();
        }

        void Submit(const voxie::RenderingConfig &config) {
            Engine::GetEngine().GetRenderingHandler()->Submit(config);
        }
    }// namespace helper

}// namespace voxie
