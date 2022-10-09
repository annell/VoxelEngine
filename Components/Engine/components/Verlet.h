#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <yaml-cpp/yaml.h>

namespace voxie {

    struct Verlet {
        glm::vec3 Directions = {0, 1, 0};
        glm::vec3 Acceleration = {0, 0, 0};
        std::optional<glm::vec3> Position_old;

        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);

        void UpdatePosition(float dt, struct Position &position_new);
        void Accelerate(const glm::vec3 &acceleration);
    };
}// namespace voxie