//
// Created by Stefan Annell on 2022-02-06.
//

#include "PhysicsHandler.h"
#include <reactphysics3d/reactphysics3d.h>

reactphysics3d::PhysicsCommon physicsCommon;

voxie::PhysicsHandler::PhysicsHandler()
    : world(physicsCommon.createPhysicsWorld()) {
    reactphysics3d::Vector3 position(0, 20, 0);
    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
    reactphysics3d::Transform transform(position, orientation);
    auto* body = world->createRigidBody(transform);
}

reactphysics3d::PhysicsWorld *voxie::PhysicsHandler::GetWorld() {
    return world;
}

void voxie::PhysicsHandler::Update(float timeStep) {
    world->update(timeStep);
}
