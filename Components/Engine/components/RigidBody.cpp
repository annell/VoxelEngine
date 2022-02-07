//
// Created by Stefan Annell on 2022-02-06.
//

#include "RigidBody.h"
#include "Core.h"
#include "reactphysics3d/reactphysics3d.h"
#include "PhysicsHandler.h"

namespace voxie {

namespace internal {
    reactphysics3d::Transform PositionToTransform(const Position& pos) {
        reactphysics3d::Transform transform;
        transform.setFromOpenGL((float*)glm::value_ptr(pos.model));
        return transform;
    }

    reactphysics3d::Collider* createCollider(reactphysics3d::RigidBody* body, const Position& pos) {
        auto* shape = getPhysicsCommon().createBoxShape({pos.scale.x/2, pos.scale.y/2, pos.scale.z/2});
        return body->addCollider(shape, reactphysics3d::Transform::identity());
    }

    reactphysics3d::RigidBody* createRigidBody(const Position& pos) {
        return Engine::GetEngine().GetPhysicsHandler().GetWorld()->createRigidBody(PositionToTransform(pos));
    }

    void TransformToPosition(const reactphysics3d::Transform &transform, Position& pos) {
        auto scale = pos.scale;
        transform.getOpenGLMatrix(glm::value_ptr(pos.model));
        pos.SetModel(pos.model);
        pos.scale = scale;
        pos.UpdateModel();
    }

    reactphysics3d::BodyType voxieToReactBodyType(BodyType bodyType) {
        switch (bodyType) {
            case BodyType::STATIC:
                return reactphysics3d::BodyType::STATIC;
            case BodyType::DYNAMIC:
                return reactphysics3d::BodyType::DYNAMIC;
            case BodyType::KINEMATIC:
                return reactphysics3d::BodyType::KINEMATIC;
        }
        return reactphysics3d::BodyType::STATIC;

    }

    BodyType reactToVoxieBodyType(reactphysics3d::BodyType bodyType) {
        switch (bodyType) {
            case reactphysics3d::BodyType::STATIC:
                return BodyType::STATIC;
            case reactphysics3d::BodyType::DYNAMIC:
                return BodyType::DYNAMIC;
            case reactphysics3d::BodyType::KINEMATIC:
                return BodyType::KINEMATIC;
        }
        return BodyType::STATIC;
    }
}

RigidBody::RigidBody(const Position& pos)
    : rigidBody(internal::createRigidBody(pos))
    , collider(nullptr) {
    collider = internal::createCollider(rigidBody, pos);
}

RigidBody::~RigidBody() {
    Engine::GetEngine().GetPhysicsHandler().GetWorld()->destroyRigidBody(rigidBody);
}

void RigidBody::encode(YAML::Node &node) const {
    node["mass"] = GetMass();
    node["gravity"] = GetGravity();
    node["bodyType"] = static_cast<int>(GetBodyType());
}

bool RigidBody::decode(const YAML::Node &node) {
    SetMass(node["mass"].as<float>());
    SetGravity(node["gravity"].as<bool>());
    SetBodyType(static_cast<voxie::BodyType>(node["bodyType"].as<int>()));
    return true;
}

void RigidBody::SetPosition(const Position& pos) const {
    assert(rigidBody);
    rigidBody->setTransform(internal::PositionToTransform(pos));
    dynamic_cast<reactphysics3d::BoxShape*>(collider->getCollisionShape())->setHalfExtents({pos.scale.x/2, pos.scale.y/2, pos.scale.z/2});
}

void RigidBody::UpdatePosition(Position &pos) const {
    assert(rigidBody);
    internal::TransformToPosition(rigidBody->getTransform(), pos);
}

BodyType RigidBody::GetBodyType() const {
    assert(rigidBody);
    return internal::reactToVoxieBodyType(rigidBody->getType());
}
void RigidBody::SetBodyType(BodyType bodyType) const {
    assert(rigidBody);
    rigidBody->setType(internal::voxieToReactBodyType(bodyType));
}
void RigidBody::SetMass(float mass) const {
    assert(rigidBody);
    rigidBody->setMass(mass);
}
float RigidBody::GetMass() const {
    assert(rigidBody);
    return rigidBody->getMass();
}

bool RigidBody::GetGravity() const {
    assert(rigidBody);
    return rigidBody->isGravityEnabled();
}

void RigidBody::SetGravity(bool gravity) const {
    assert(rigidBody);
    rigidBody->enableGravity(gravity);
}

}
