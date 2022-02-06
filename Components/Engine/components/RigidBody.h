//
// Created by Stefan Annell on 2022-02-06.
//

#pragma once
#include <yaml-cpp/yaml.h>

namespace reactphysics3d {
    class RigidBody;
}

namespace voxie {
struct Position;


struct Body {
    Body(const Position&);

    void encode(YAML::Node &node) const;
    bool decode(const YAML::Node &node);

    void SetPosition(const Position&) const;
    void UpdatePosition(Position&) const;

    reactphysics3d::RigidBody* rigidBody;
};
}
