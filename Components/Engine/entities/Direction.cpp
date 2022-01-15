//
// Created by Stefan Annell on 2022-01-11.
//

#include "Direction.h"

namespace voxie {
    Direction::Direction(float yaw, float pitch)
            : yaw(yaw), pitch(pitch) {
    }

    void Direction::encode(YAML::Node &node) const {
        node["yaw"] = yaw;
        node["pitch"] = pitch;
    }

    bool Direction::decode(const YAML::Node &node) {
        yaw = node["yaw"].as<float>();
        pitch = node["pitch"].as<float>();
        return true;
    }

    void Direction::SetDirection(float yaw, float pitch, bool constrainPitch) {
        this->yaw = yaw;
        this->pitch = pitch;

        if (constrainPitch) {
            if (this->pitch > 89.0f)
                this->pitch = 89.0f;
            if (this->pitch < -89.0f)
                this->pitch = -89.0f;
        }
    }

}
