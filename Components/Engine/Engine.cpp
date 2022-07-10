#include "Engine.h"
#include "Core.h"
#include <iostream>

#define GL_SILENCE_DEPRECATION
#include "CubeEntity.h"
#include "GameMode.h"
#include "Outline.h"
#include "Text.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <RigidBody.h>

namespace voxie {
    const unsigned int SCR_WIDTH = 1024;
    const unsigned int SCR_HEIGHT = 768;

    Engine::Engine()
        : scene(std::make_unique<Scene>(BASE_PATH + SCENES + "/")), camera(NullEntity), gameMode(nullptr), textHandler(nullptr) {
    }

    Engine::~Engine() {
        glfwTerminate();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
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
        InitGUI();
        GetPhysicsHandler()->Initialize();
        return true;
    }

    bool Engine::InitWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = std::make_shared<Window>(glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Voxie", nullptr, nullptr), SCR_WIDTH,
                                          SCR_HEIGHT);
        if (window == nullptr) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window->GetWindow());
        glfwSetFramebufferSizeCallback(window->GetWindow(), MouseHandler::framebuffer_size_callback);
        glfwSetCursorPosCallback(window->GetWindow(), MouseHandler::mouse_movement_callback);
        glfwSetScrollCallback(window->GetWindow(), MouseHandler::scroll_callback);
        glfwSetMouseButtonCallback(window->GetWindow(), MouseHandler::mouse_clicked_callback);
        glfwSwapInterval(1);

        glewInit();

        glEnable(GL_DEPTH_TEST);

        textHandler = std::make_unique<TextHandler>(SCR_WIDTH, SCR_HEIGHT, BASE_PATH + FONTS + "/Arial.ttf", std::make_shared<voxie::Shader>(std::map<std::string, unsigned int>{std::make_pair(BASE_PATH + SHADERS + "/text.vs", GL_VERTEX_SHADER), std::make_pair(BASE_PATH + SHADERS + "/text.fs", GL_FRAGMENT_SHADER)}));
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

            NewFrame();

            KeyboardHandler::processInput();
            voxie::helper::RenderingBegin();
            onTick.Broadcast(GetDeltaTime());
            SubmitNodesForRendering(GetScene()->GetNodesForRendering());

            RenderFrame();
        }
    }

    void Engine::StopLoop() {
        isRunning = false;
    }

    bool Engine::IsRunning() const {
        return isRunning && !glfwWindowShouldClose(window->GetWindow());
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

    void Engine::RenderFrame() const {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }

    void Engine::NewFrame() const {
        glClearColor(0.25f, 0.6f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Engine::UpdateTime() {
        float currentFrame = glfwGetTime();
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

    void Engine::InitGUI() const {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui_ImplGlfw_InitForOpenGL(GetWindow()->GetWindow(), true);
        ImGui_ImplOpenGL3_Init(nullptr);
        ImGui::StyleColorsDark();
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
