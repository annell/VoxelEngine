#include "Verlet.h"
#include "Position.h"

namespace voxie {
    void Verlet::encode(YAML::Node &node) const {
        YAML::Node directionNode;
        directionNode["x"] = Directions[0];
        directionNode["y"] = Directions[1];
        directionNode["z"] = Directions[2];
        node["directions"] = directionNode;
    }

    bool Verlet::decode(const YAML::Node &node) {
        auto directions = node["directions"];
        if (directions.IsDefined()) {
            Directions = {directions["x"].as<float>(),
                          directions["y"].as<float>(),
                          directions["z"].as<float>()};
        }
        return true;
    }

    void Verlet::UpdatePosition(float dt, Position &position_new) {
        if (!Position_old) {
            Position_old = position_new.pos;
            return;
        }
        const glm::vec3 velocity = Directions * (position_new.pos - Position_old.value());
        Position_old = position_new.pos;
        position_new.pos += velocity + Acceleration * dt * dt;
        Acceleration = {0, 0, 0};
    }

    void Verlet::Accelerate(const glm::vec3 &acceleration) {
        Acceleration += acceleration;
    }
}// namespace voxie