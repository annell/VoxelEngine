#pragma once

#include "JsonUtil.h"
#include <optional>

namespace voxie {

    struct Verlet {
        glm::vec3 Directions = {0, 1, 0};
        glm::vec3 Acceleration = {0, 0, 0};
        std::optional<glm::vec3> PositionOld;
        glm::vec3 dVelocity = {0, 0, 0};
        glm::vec3 Movement = {0, 0, 0};
        glm::vec3 MovementVelocity = {0, 0, 0};
        bool OnGround = false;
        bool dynamic = false;

        void UpdatePosition(float dt, struct Position &position_new);
        void UpdateMovementPosition(float dt, struct Position &position_new);
        void UpdateOldPosition(const Position &);
        void Accelerate(const glm::vec3 &acceleration);
    };
}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::Verlet> {
        static Node encode(const voxie::Verlet &rhs) {
            Node node;
            node["dynamic"] = rhs.dynamic;
            node["directions"] = rhs.Directions;
            return node;
        }

        static bool decode(const Node &node, voxie::Verlet &rhs) {
            if (!node["dynamic"].IsDefined() || !node["directions"].IsDefined()) {
                return false;
            }

            rhs.dynamic = node["dynamic"].as<bool>(false);
            rhs.Directions = node["directions"].as<glm::vec3>(glm::vec3{});
            return true;
        }
    };
}// namespace YAML