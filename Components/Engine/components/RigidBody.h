//
// Created by Stefan Annell on 2022-02-06.
//

#pragma once
#include <yaml-cpp/yaml.h>

namespace reactphysics3d {
    class RigidBody;
    class Collider;
}

namespace voxie {
    enum class BodyType {
        STATIC,
        KINEMATIC,
        DYNAMIC
    };

struct Position;
struct Body {
    Body(const Position&);
    ~Body();

    void encode(YAML::Node &node) const;
    bool decode(const YAML::Node &node);

    void SetPosition(const Position&) const;
    void UpdatePosition(Position&) const;

    BodyType GetBodyType() const;
    void SetBodyType(BodyType) const;

    float GetMass() const;
    void SetMass(float) const;

    bool GetGravity() const;
    void SetGravity(bool) const;

    reactphysics3d::RigidBody* rigidBody;
    reactphysics3d::Collider* collider;
};
}
