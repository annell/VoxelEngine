//
// Created by Stefan Annell on 2022-02-06.
//

#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "EntityComponentSystem.h"
#include <glm/glm.hpp>

namespace reactphysics3d {
    class PhysicsWorld;
    class PhysicsCommon;
}// namespace reactphysics3d

namespace voxie {

    struct Ray {
        glm::vec3 startPoint;
        glm::vec3 endPoint;

        float maxFraction = 1.0f;
    };

    struct RaycastInfo {
        glm::vec3 worldPoint;
        glm::vec3 worldNormal;
        uint32_t collisionId;
    };

    class voxieCallbackClass;

    class PhysicsHandler {
    public:
        using raycastCallback = std::function<void(const RaycastInfo &)>;
        PhysicsHandler();
        void Update(float timeStep);
        reactphysics3d::PhysicsWorld *GetWorld();
        void RayCast(const Ray &, raycastCallback);

    private:
        reactphysics3d::PhysicsWorld *world;
    };

    reactphysics3d::PhysicsCommon &getPhysicsCommon();

}// namespace voxie
