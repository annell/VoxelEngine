//
// Created by Stefan Annell on 2022-02-06.
//

#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "EntityComponentSystem.h"
#include <glm/glm.hpp>

namespace reactphysics3d {
    class PhysicsWorld;
    class PhysicsCommon;
}

namespace voxie {

struct Ray {
    glm::vec3 point1;
    glm::vec3 point2;

    float maxFraction;
};

struct RaycastInfo {
    glm::vec3 worldPoint;
    glm::vec3 worldNormal;
};

class voxieCallbackClass;

class PhysicsHandler {
public:
    using raycastCallback = std::function<void(const RaycastInfo&)>;
    PhysicsHandler();
    void Update(float timeStep);
    reactphysics3d::PhysicsWorld* GetWorld();
    void RayCast(const Ray&, raycastCallback);

private:
    reactphysics3d::PhysicsWorld* world;
    std::vector<voxieCallbackClass*> pendingCallbacks;
};

reactphysics3d::PhysicsCommon& getPhysicsCommon();

}
