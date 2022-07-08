//
// Created by Stefan Annell on 2022-02-06.
//

#pragma once
#include <glm/glm.hpp>
#include <limits>
#include <yaml-cpp/yaml.h>

namespace reactphysics3d {
    class RigidBody;
    class Collider;
}// namespace reactphysics3d

namespace voxie {
    enum class BodyType {
        STATIC,
        KINEMATIC,
        DYNAMIC
    };

    struct Position;

    reactphysics3d::Collider *CreateBoxCollider(reactphysics3d::RigidBody *body, const Position &pos);

    struct ChunkAxis {
        float max = std::numeric_limits<float>::lowest();
        float min = std::numeric_limits<float>::max();

        float diff() const {
            return max - min;
        }

        float shift() const {
            return min;
        }
    };

    struct RigidBody {
        RigidBody(const Position &);
        ~RigidBody();

        void BeginPlay() const;

        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);

        void SetPosition(const Position &) const;
        void GetPosition(Position &) const;

        void SetOffset(const glm::vec3 &);
        glm::vec3 GetOffset() const;
        Position GetPositionWithOffset(const Position &) const;

        BodyType GetBodyType() const;
        void SetBodyType(BodyType);

        float GetMass() const;
        void SetMass(float) const;

        bool GetGravity() const;
        void SetGravity(bool) const;

        glm::vec3 GetLinearAxisFactor() const;
        void SetLinearAxisFactor(const glm::vec3 &) const;

        glm::vec3 GetAngularAxisFactor() const;
        void SetAngularAxisFactor(const glm::vec3 &) const;

        uint32_t GetColliderId() const;

        void ApplyForceAtCenterOfMass(const glm::vec3 &) const;
        void ResetForces() const;
        glm::vec3 GetForces() const;

        float GetLinearDampening() const;
        void SetLinearDampening(float) const;

        float GetBounciness() const;
        void SetBounciness(float) const;

        reactphysics3d::RigidBody *rigidBody;
        reactphysics3d::Collider *collider;

        glm::vec3 offset = {0, 0, 0};
        BodyType bodyType = BodyType::STATIC;
    };
}// namespace voxie
