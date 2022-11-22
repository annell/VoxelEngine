//
// Created by Stefan Annell on 2022-05-17.
//

#include "EntityComponentSystem.h"
#include <gtest/gtest.h>

TEST(ECS, RegisterECS) {
    voxie::ECSManager ecs;
    ecs.RegisterComponent<std::string>();
    ecs.RegisterComponent<int>();
    voxie::Handle handle(1);
    voxie::Handle handle2(2);
    ecs.Add(handle, std::make_shared<std::string>("Hej"));
    ecs.Add(handle2, std::make_shared<int>(5));


    ASSERT_TRUE(ecs.Has<std::string>(handle));
    ASSERT_FALSE(ecs.Has<int>(handle));

    ASSERT_FALSE(ecs.Has<std::string>(handle2));
    ASSERT_TRUE(ecs.Has<int>(handle2));
}

TEST(ECS, AddECS) {
    voxie::ECSManager ecs;
    ecs.RegisterComponent<int>();
    voxie::Handle handle(1);
    voxie::Handle handle2(2);
    ecs.Add(handle2, std::make_shared<int>(5));


    ASSERT_FALSE(ecs.Has<int>(handle));

    ASSERT_TRUE(ecs.Has<int>(handle2));
    ASSERT_EQ(*ecs.Get<int>(handle2).get(), 5);

    ecs.Add(handle, std::make_shared<int>(4));
    ASSERT_EQ(*ecs.Get<int>(handle).get(), 4);
    ASSERT_EQ(*ecs.Get<int>(handle2).get(), 5);

    ecs.Add(handle, std::make_shared<int>(9));
    ASSERT_EQ(*ecs.Get<int>(handle).get(), 9);
}

TEST(ECS, RemoveECS) {
    voxie::ECSManager ecs;
    ecs.RegisterComponent<int>();
    voxie::Handle handle(1);

    ASSERT_FALSE(ecs.Has<int>(handle));
    ecs.Add(handle, std::make_shared<int>(5));
    ASSERT_TRUE(ecs.Has<int>(handle));
    ecs.Remove<int>(handle);
    ASSERT_FALSE(ecs.Has<int>(handle));
}

TEST(ECS2, RegisterECS) {
    voxie::ECSManagerArenaAllocator ecs;
    voxie::Handle handle(1);

    ASSERT_DEATH({ ecs.Add(handle, std::string("Hej")); }, "Component not registered before use.*");
    ecs.RegisterComponent<std::string>();
    ecs.Add(handle, std::string("Hej"));

    ASSERT_TRUE(ecs.Has<std::string>(handle));
    ecs.RegisterComponent<int>();
    ASSERT_DEATH({ ecs.RegisterComponent<int>(); }, "Registering component type more than once.*");
    ASSERT_FALSE(ecs.Has<int>(handle));
}

TEST(ECS2, AddECS) {
    voxie::ECSManagerArenaAllocator ecs;
    ecs.RegisterComponent<int>();
    voxie::Handle handle(1);
    voxie::Handle handle2(2);
    ecs.Add(handle2, 5);


    ASSERT_FALSE(ecs.Has<int>(handle));

    ASSERT_TRUE(ecs.Has<int>(handle2));
    ASSERT_EQ(*ecs.Get<int>(handle2), 5);

    ecs.Add(handle, 4);
    ASSERT_EQ(*ecs.Get<int>(handle), 4);
    ASSERT_EQ(*ecs.Get<int>(handle2), 5);

    ecs.Add(handle, 9);
    ASSERT_EQ(*ecs.Get<int>(handle2), 5);
    ASSERT_EQ(*ecs.Get<int>(handle), 4);
    ecs.Remove<int>(handle);
    ecs.Add(handle, 9);
    ASSERT_EQ(*ecs.Get<int>(handle), 9);
}

TEST(ECS2, RemoveECS) {
    voxie::ECSManagerArenaAllocator ecs;
    ecs.RegisterComponent<int>();
    voxie::Handle handle(1);

    ASSERT_FALSE(ecs.Has<int>(handle));
    ecs.Add(handle, 5);
    ASSERT_TRUE(ecs.Has<int>(handle));
    ecs.Remove<int>(handle);
    ASSERT_FALSE(ecs.Has<int>(handle));
    ASSERT_FALSE(ecs.Get<int>(handle).has_value());
}


#include <chrono>

TEST(ECS2, speeed) {
    using namespace std::chrono;
    int max = 1000000;

    auto start1 = high_resolution_clock::now();
    {
        voxie::ECSManager ecs1;
        ecs1.RegisterComponent<int>();
        for (int i = 0; i < max; i++) {
            ecs1.Add(voxie::Handle::MakeEntity(), std::make_shared<int>(5));
        }
    }
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1);


    auto start2 = high_resolution_clock::now();
    {
        voxie::ECSManagerArenaAllocator ecs2;
        ecs2.RegisterComponent<int>();
        for (int i = 0; i < max; i++) {
            ecs2.Add(voxie::Handle::MakeEntity(), 5);
        }
    }
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2);

    ASSERT_EQ(duration1.count(), duration2.count());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}