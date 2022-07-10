//
// Created by Stefan Annell on 2022-05-17.
//

#include "Engine.h"
#include "Factory.h"
#include "GameMode.h"
#include <gtest/gtest.h>

TEST(Engine, Construction) {
    using engine = voxie::Engine;
    ASSERT_FALSE(std::is_constructible<engine>::value);
    ASSERT_FALSE(std::is_destructible<engine>::value);
    ASSERT_FALSE((std::is_assignable<engine, engine>::value));
    ASSERT_FALSE(std::is_copy_assignable<engine>::value);
    ASSERT_FALSE(std::is_copy_constructible<engine>::value);
    ASSERT_FALSE(std::is_move_assignable<engine>::value);
    ASSERT_FALSE(std::is_move_constructible<engine>::value);
}

TEST(Engine, GetCamera) {
    auto &engine = voxie::Engine::GetEngine();
    auto camera = engine.GetCamera();
    ASSERT_FALSE(camera);

    camera = voxie::MakeCamera({});
    engine.GetScene()->AddNode(camera, nullptr);
    engine.SetCamera(camera->GetHandle());

    auto camera2 = engine.GetCamera();
    ASSERT_TRUE(camera2);
    ASSERT_EQ(camera, camera2);
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

    engine.SetGameMode(std::make_unique<voxie::GameMode>());
    gameMode = engine.GetGameMode();
    ASSERT_TRUE(gameMode);
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

TEST(Engine, Loop) {
    auto &engine = voxie::Engine::GetEngine();
    ASSERT_FALSE(engine.IsRunning());
    engine.onTick.Bind([&](auto) {
        ASSERT_TRUE(engine.IsRunning());
        voxie::Engine::GetEngine().StopLoop();
    });
    engine.StartLoop();
    ASSERT_FALSE(engine.IsRunning());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}