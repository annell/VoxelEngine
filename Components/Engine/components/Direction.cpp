//
// Created by Stefan Annell on 2022-01-11.
//

#include "Direction.h"

namespace voxie {
    Direction::Direction() : Direction(0.0f, 0.0f) {}
    Direction::Direction(float yaw, float pitch)
        : yaw(yaw), pitch(pitch) {
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

}// namespace voxie
