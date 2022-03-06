//
// Created by Stefan Annell on 2022-02-06.
//

#include "PhysicsHandler.h"
#include <reactphysics3d/reactphysics3d.h>
#include "Core.h"

namespace voxie {

namespace internal {

reactphysics3d::Vector3 glmToReactVector(const glm::vec3& vector) {
    return reactphysics3d::Vector3(vector.x, vector.y, vector.z);
}

glm::vec3 reactVectorToGlm(const reactphysics3d::Vector3& vector) {
    return glm::vec3(vector.x, vector.y, vector.z);
}

}

class voxieCallbackClass : public reactphysics3d::RaycastCallback {
public:
    voxieCallbackClass(PhysicsHandler::raycastCallback callback)
    : callback(callback) {

    }

    reactphysics3d::decimal notifyRaycastHit(const reactphysics3d::RaycastInfo& info) override {
        voxie::RaycastInfo rayInfo = {
                internal::reactVectorToGlm(info.worldNormal),
                internal::reactVectorToGlm(info.worldPoint)
        };
        callback(rayInfo);
        return reactphysics3d::decimal(0.0);
    }
private:
    PhysicsHandler::raycastCallback callback;
};

reactphysics3d::PhysicsCommon& getPhysicsCommon() {
    static reactphysics3d::PhysicsCommon physicsCommon;
    return physicsCommon;
}

PhysicsHandler::PhysicsHandler()
    : world(getPhysicsCommon().createPhysicsWorld()) {
}

reactphysics3d::PhysicsWorld *PhysicsHandler::GetWorld() {
    return world;
}

void PhysicsHandler::Update(float timeStep) {
    world->update(timeStep);
}

void PhysicsHandler::RayCast(const Ray& ray, PhysicsHandler::raycastCallback callback) {
    reactphysics3d::Ray reactRay(internal::glmToReactVector(ray.point1), internal::glmToReactVector(ray.point2), ray.maxFraction);
    pendingCallbacks.push_back(new voxieCallbackClass(callback));
    world->raycast(reactRay, pendingCallbacks.back());
}

}
