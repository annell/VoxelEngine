//
// Created by Stefan Annell on 2022-02-06.
//

#include "RigidBody.h"
#include "Core.h"
#include "GameMode.h"
#include "PhysicsHandler.h"
#include "reactphysics3d/reactphysics3d.h"

namespace voxie {

    namespace internal {
        reactphysics3d::Transform PositionToTransform(const Position &pos) {
            reactphysics3d::Transform transform;
            transform.setFromOpenGL((float *) glm::value_ptr(pos.model));
            return transform;
        }

        void TransformToPosition(const reactphysics3d::Transform &transform, Position &pos) {
            glm::mat4 model;
            transform.getOpenGLMatrix(glm::value_ptr(model));
            pos.SetModel(glm::scale(model, pos.scale));
        }

        reactphysics3d::RigidBody *createRigidBody(const Position &pos) {
            return Engine::GetEngine().GetPhysicsHandler()->GetWorld()->createRigidBody(PositionToTransform(pos));
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

        reactphysics3d::Vector3 getHalfExtens(const Position &pos) {
            return {pos.scale.x / 2, pos.scale.y / 2, pos.scale.z / 2};
        }
    }// namespace internal

    reactphysics3d::Collider *CreateBoxCollider(reactphysics3d::RigidBody *body, const Position &pos) {
        auto *shape = getPhysicsCommon().createBoxShape(internal::getHalfExtens(pos));
        return body->addCollider(shape, reactphysics3d::Transform::identity());
    }

    RigidBody::RigidBody(const Position &pos)
        : rigidBody(internal::createRigidBody(pos)), collider(nullptr) {
        rigidBody->setType(internal::voxieToReactBodyType(BodyType::STATIC));
    }

    RigidBody::~RigidBody() {
        Engine::GetEngine().GetPhysicsHandler()->GetWorld()->destroyRigidBody(rigidBody);
    }

    void RigidBody::BeginPlay() const {
        rigidBody->setType(internal::voxieToReactBodyType(bodyType));
    }

    void RigidBody::encode(YAML::Node &node) const {
        node["mass"] = GetMass();
        node["gravity"] = GetGravity();
        node["bodyType"] = static_cast<int>(GetBodyType());

        YAML::Node offsetNode;
        offsetNode["x"] = offset[0];
        offsetNode["y"] = offset[1];
        offsetNode["z"] = offset[2];
        node["offset"] = offsetNode;

        YAML::Node linearLockNode;
        auto linearLockFactor = rigidBody->getLinearLockAxisFactor();
        linearLockNode["x"] = linearLockFactor[0];
        linearLockNode["y"] = linearLockFactor[1];
        linearLockNode["z"] = linearLockFactor[2];
        node["linearLockFactor"] = linearLockNode;

        YAML::Node angularLockNode;
        auto angularLockFactor = rigidBody->getAngularLockAxisFactor();
        angularLockNode["x"] = angularLockFactor[0];
        angularLockNode["y"] = angularLockFactor[1];
        angularLockNode["z"] = angularLockFactor[2];
        node["angularLockFactor"] = angularLockNode;
    }

    bool RigidBody::decode(const YAML::Node &node) {
        auto offsetNode = node["offset"];
        SetOffset({offsetNode["x"].as<float>(),
                   offsetNode["y"].as<float>(),
                   offsetNode["z"].as<float>()});
        SetMass(node["mass"].as<float>());
        SetGravity(node["gravity"].as<bool>());
        SetBodyType(static_cast<voxie::BodyType>(node["bodyType"].as<int>()));
        rigidBody->setType(internal::voxieToReactBodyType(BodyType::STATIC));

        auto linearLock = node["linearLockFactor"];
        SetLinearAxisFactor({linearLock["x"].as<float>(),
                             linearLock["y"].as<float>(),
                             linearLock["z"].as<float>()});

        auto angularLock = node["angularLockFactor"];
        SetAngularAxisFactor({angularLock["x"].as<float>(),
                              angularLock["y"].as<float>(),
                              angularLock["z"].as<float>()});
        return true;
    }

    void RigidBody::SetPosition(const Position &pos) const {
        assert(rigidBody && collider);
        rigidBody->setTransform(internal::PositionToTransform(GetPositionWithOffset(pos)));
        dynamic_cast<reactphysics3d::BoxShape *>(collider->getCollisionShape())->setHalfExtents(internal::getHalfExtens(pos));
        rigidBody->resetForce();
        rigidBody->resetTorque();
    }

    Position RigidBody::GetPositionWithOffset(const Position &pos) const {
        auto modifiedpos = pos;
        modifiedpos.pos += GetOffset();
        modifiedpos.UpdateModel();
        return modifiedpos;
    }

    void RigidBody::SetOffset(const glm::vec3 &offset) {
        this->offset = offset;
    }

    glm::vec3 RigidBody::GetOffset() const {
        return offset;
    }

    glm::vec3 RigidBody::GetLinearAxisFactor() const {
        auto factor = rigidBody->getLinearLockAxisFactor();
        return {factor.x, factor.y, factor.z};
    }
    void RigidBody::SetLinearAxisFactor(const glm::vec3 &factor) const {
        rigidBody->setLinearLockAxisFactor({factor.x, factor.y, factor.z});
    }

    glm::vec3 RigidBody::GetAngularAxisFactor() const {
        auto factor = rigidBody->getAngularLockAxisFactor();
        return {factor.x, factor.y, factor.z};
    }
    void RigidBody::SetAngularAxisFactor(const glm::vec3 &factor) const {
        rigidBody->setAngularLockAxisFactor({factor.x, factor.y, factor.z});
    }

    void RigidBody::GetPosition(Position &pos) const {
        assert(rigidBody);
        internal::TransformToPosition(rigidBody->getTransform(), pos);
        pos.pos -= GetOffset();
        pos.UpdateModel();
    }

    BodyType RigidBody::GetBodyType() const {
        return bodyType;
    }
    void RigidBody::SetBodyType(BodyType type) {
        bodyType = type;
        if (Engine::GetEngine().GetGameMode()->IsStarted()) {
            rigidBody->setType(internal::voxieToReactBodyType(bodyType));
        }
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

    uint32_t RigidBody::GetColliderId() const {
        return rigidBody->getEntity().getIndex();
    }

    void RigidBody::ApplyForceAtCenterOfMass(const glm::vec3 &force) const {
        rigidBody->applyWorldForceAtCenterOfMass({force.x, force.y, force.z});
    }

    void RigidBody::ResetForces() const {
        rigidBody->resetForce();
    }

    glm::vec3 RigidBody::GetForces() const {
        auto forces = rigidBody->getForce();
        return {forces.x, forces.y, forces.z};
    }

    float RigidBody::GetLinearDampening() const {
        return rigidBody->getLinearDamping();
    }

    void RigidBody::SetLinearDampening(float factor) const {
        rigidBody->setLinearDamping(factor);
    }

    float RigidBody::GetBounciness() const {
        return collider->getMaterial().getBounciness();
    }

    void RigidBody::SetBounciness(float factor) const {
        collider->getMaterial().setBounciness(factor);
    }

}// namespace voxie
