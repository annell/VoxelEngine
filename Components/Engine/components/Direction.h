//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include "JsonUtil.h"

namespace voxie {
    struct Direction {
        Direction();
        Direction(float yaw, float pitch);

        void SetDirection(float yaw, float pitch, bool constrainPitch);

        float yaw;
        float pitch;
    };

}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::Direction> {
        static Node encode(const voxie::Direction &rhs) {
            Node node;
            node["yaw"] = rhs.yaw;
            node["pitch"] = rhs.pitch;
            return node;
        }

        static bool decode(const Node &node, voxie::Direction &rhs) {
            if (!node["yaw"].IsDefined() && !node["pitch"].IsDefined()) {
                return false;
            }

            rhs.yaw = node["yaw"].as<float>(0.0f);
            rhs.pitch = node["pitch"].as<float>(0.0f);
            return true;
        }
    };
}// namespace YAML
