//
// Created by Stefan Annell on 2022-02-06.
//

#pragma once
#include <glm/glm.hpp>
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
    reactphysics3d::Collider *CreateConvexCollider(reactphysics3d::RigidBody *body, const std::vector<float> &vertices, const Position &pos);

    struct ChunkMaxMin {
        float max = -999999;
        float min = 9999999;
        float diff() const {
            return max - min;
        }
    };

    constexpr ChunkMaxMin ChunkDefaultSize = {1, 0};
    constexpr std::array<ChunkMaxMin, 3> DefaultMaxMin = {ChunkDefaultSize, ChunkDefaultSize, ChunkDefaultSize};

    struct RigidBody {
        RigidBody(const Position &);
        ~RigidBody();

        void BeginPlay() const;

        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);

        void SetPosition(const Position &) const;
        void UpdatePosition(Position &) const;

        BodyType GetBodyType() const;
        void SetBodyType(BodyType);

        float GetMass() const;
        void SetMass(float) const;

        bool GetGravity() const;
        void SetGravity(bool) const;

        uint32_t GetColliderId() const;

        reactphysics3d::RigidBody *rigidBody;
        reactphysics3d::Collider *collider;

        BodyType bodyType;
        std::array<ChunkMaxMin, 3> maxMin = DefaultMaxMin;
    };
}// namespace voxie
