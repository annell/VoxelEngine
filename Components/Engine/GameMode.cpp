//
// Created by Stefan Annell on 2022-01-29.
//

#include "GameMode.h"
#include "Core.h"

namespace voxie {

    GameMode::GameMode()
        : GameMode("MainScene.voxie") {
    }

    GameMode::GameMode(const std::string &sceneSaveFile)
        : sceneSavefile(sceneSaveFile) {
        auto &engine = Engine::GetEngine();
        OnTickHandle = engine.onTick.Bind(std::bind(&GameMode::OnTick, this, std::placeholders::_1));
    }

    GameMode::~GameMode() {
        auto &engine = Engine::GetEngine();
        engine.onTick.Unbind(OnTickHandle);
    }

    void GameMode::Reset() {
        auto &engine = Engine::GetEngine();
        auto &scene = engine.GetScene();
        scene.ClearScene();
        scene.Load(sceneSavefile);
        if (scene.GetEntities().empty()) {
            auto camera = MakeCamera({"Editor Camera"});
            auto entity = camera->GetHandle();
            scene.AddNode(camera, nullptr);
            engine.SetCamera(entity);
        }
        Stop();
    }

    void GameMode::Start() {
        started = true;
    }

    void GameMode::Stop() {
        started = false;
    }

    void GameMode::Resume() {
        started = true;
    }

    void GameMode::OnTick(float deltaTime) {
    }

    bool GameMode::IsStarted() const {
        return started;
    }

}// namespace voxie
