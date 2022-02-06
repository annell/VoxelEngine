//
// Created by Stefan Annell on 2022-02-06.
//

#include "RigidBody.h"
#include "Core.h"
#include "reactphysics3d/reactphysics3d.h"

namespace voxie {

namespace internal {
    reactphysics3d::Transform PositionToTransform(const Position& pos) {
        reactphysics3d::Vector3 position(pos.pos.x, pos.pos.y, pos.pos.z);
        reactphysics3d::Quaternion orientation(pos.rotationQuat.x, pos.rotationQuat.y, pos.rotationQuat.z, pos.rotationQuat.w);
        return reactphysics3d::Transform(position, orientation);
    }

    reactphysics3d::RigidBody* createRigidBody(const Position& pos) {
        return Engine::GetEngine().GetPhysicsHandler().GetWorld()->createRigidBody(PositionToTransform(pos));
    }

    void TransformToPosition(const reactphysics3d::Transform &transform, Position& pos) {
        auto position = transform.getPosition();
        auto orientation = transform.getOrientation();

        pos.pos = {position.x, position.y, position.z};
        pos.rotationQuat = {orientation.x, orientation.y, orientation.z, orientation.w};
    }
}

Body::Body(const Position& pos)
    : rigidBody(internal::createRigidBody(pos)) {
}

void Body::encode(YAML::Node &node) const {

}

bool Body::decode(const YAML::Node &node) {
    return false;
}

void Body::SetPosition(const Position& pos) const {
    assert(rigidBody);
    rigidBody->setTransform(internal::PositionToTransform(pos));
}

void Body::UpdatePosition(Position &pos) const {
    assert(rigidBody);
    internal::TransformToPosition(rigidBody->getTransform(), pos);
}

}
