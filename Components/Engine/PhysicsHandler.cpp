//
// Created by Stefan Annell on 2022-02-06.
//

#include "PhysicsHandler.h"
#include <reactphysics3d/reactphysics3d.h>

namespace voxie {


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

}
