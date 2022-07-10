//
// Created by Stefan Annell on 2022-05-17.
//

#include "Engine.h"
#include <gtest/gtest.h>

TEST(Engine, Init) {
    auto &engine = voxie::Engine::GetEngine();
    ASSERT_TRUE(engine.Init());
}

TEST(Engine, GetCamera) {
    auto &engine = voxie::Engine::GetEngine();
    auto camera = engine.GetCamera();
    ASSERT_FALSE(camera);
}

TEST(Engine, GetWindow) {
    auto &engine = voxie::Engine::GetEngine();
    auto window = engine.GetWindow();
    ASSERT_TRUE(window);
}

TEST(Engine, GetScene) {
    auto &engine = voxie::Engine::GetEngine();
    auto scene = engine.GetScene();
    ASSERT_TRUE(scene);
}

TEST(Engine, GetECSManager) {
    auto &engine = voxie::Engine::GetEngine();
    auto ecs = engine.GetECSManager();
    ASSERT_TRUE(ecs);
}

TEST(Engine, GetGameMode) {
    auto &engine = voxie::Engine::GetEngine();
    auto gameMode = engine.GetGameMode();
    ASSERT_FALSE(gameMode);
}

TEST(Engine, GetPhysicsHandler) {
    auto &engine = voxie::Engine::GetEngine();
    auto physicsHandler = engine.GetPhysicsHandler();
    ASSERT_TRUE(physicsHandler);
}

TEST(Engine, GetRenderingHandler) {
    auto &engine = voxie::Engine::GetEngine();
    auto renderingHandler = engine.GetRenderingHandler();
    ASSERT_TRUE(renderingHandler);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}