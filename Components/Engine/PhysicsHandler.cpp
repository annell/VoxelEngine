//
// Created by Stefan Annell on 2022-02-06.
//

#include "PhysicsHandler.h"
#include "Core.h"
#include <reactphysics3d/reactphysics3d.h>

namespace voxie {

    namespace internal {

        reactphysics3d::Vector3 glmToReactVector(const glm::vec3 &vector) {
            return reactphysics3d::Vector3(vector.x, vector.y, vector.z);
        }

        glm::vec3 reactVectorToGlm(const reactphysics3d::Vector3 &vector) {
            return glm::vec3(vector.x, vector.y, vector.z);
        }

    }// namespace internal

    class voxieCallbackClass : public reactphysics3d::RaycastCallback {
    public:
        voxieCallbackClass(PhysicsHandler::raycastCallback callback)
            : callback(callback) {
        }

        reactphysics3d::decimal notifyRaycastHit(const reactphysics3d::RaycastInfo &info) override {
            voxie::RaycastInfo rayInfo = {
                    internal::reactVectorToGlm(info.worldPoint),
                    internal::reactVectorToGlm(info.worldNormal),
                    info.body->getEntity().getIndex()};
            callback(rayInfo);
            return reactphysics3d::decimal(0.0);
        }

    private:
        PhysicsHandler::raycastCallback callback;
    };

    reactphysics3d::PhysicsCommon &getPhysicsCommon() {
        static reactphysics3d::PhysicsCommon physicsCommon;
        return physicsCommon;
    }

    PhysicsHandler::PhysicsHandler()
        : world(getPhysicsCommon().createPhysicsWorld()) {

        // Create the default logger
        reactphysics3d::DefaultLogger *logger = getPhysicsCommon().createDefaultLogger();

        // Log level (warnings and errors)
        uint logLevel = static_cast<uint>(
                static_cast<uint>(reactphysics3d::Logger::Level::Warning) |
                static_cast<uint>(reactphysics3d::Logger::Level::Error) |
                static_cast<uint>(reactphysics3d::Logger::Level::Information));

        // Output the logs into the standard output
        logger->addStreamDestination(std::cout, logLevel, reactphysics3d::DefaultLogger::Format::Text);

        // Set the logger
        getPhysicsCommon().setLogger(logger);
    }

    reactphysics3d::PhysicsWorld *PhysicsHandler::GetWorld() {
        return world;
    }

    void PhysicsHandler::Update(float timeStep) {
        static const float timeStepFraction = 1.0f / 60.0f;
        static float accumulator = 0.0f;
        accumulator += timeStep;

        if (accumulator >= timeStepFraction) {
            world->update(timeStepFraction);
            accumulator -= timeStepFraction;
        }
    }

    void PhysicsHandler::RayCast(const Ray &ray, PhysicsHandler::raycastCallback callback) {
        reactphysics3d::Ray reactRay(internal::glmToReactVector(ray.startPoint), internal::glmToReactVector(ray.endPoint), ray.maxFraction);
        voxieCallbackClass voxieCallback(callback);
        world->raycast(reactRay, &voxieCallback);
    }

}// namespace voxie
