//
// Created by Stefan Annell on 2022-02-06.
//

#pragma once

#include <map>

#include "EntityComponentSystem.h"

namespace reactphysics3d {
    class PhysicsWorld;
    class PhysicsCommon;
}

namespace voxie {

class PhysicsHandler {
public:
    PhysicsHandler();
    void Update(float timeStep);
    reactphysics3d::PhysicsWorld* GetWorld();

private:
    reactphysics3d::PhysicsWorld* world;
};

reactphysics3d::PhysicsCommon& getPhysicsCommon();

}
