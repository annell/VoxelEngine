//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include <yaml-cpp/yaml.h>

namespace voxie {
    struct Direction {
        Direction(float yaw, float pitch);

        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);

        void SetDirection(float yaw, float pitch, bool constrainPitch);

        float yaw;
        float pitch;
    };

}// namespace voxie
