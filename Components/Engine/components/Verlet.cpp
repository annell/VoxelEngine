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

    void Verlet::UpdatePosition(float dt, Position &positionNew) {
        if (!PositionOld) {
            PositionOld = positionNew.pos;
            return;
        }
        const glm::vec3 velocity = Directions * (positionNew.pos - PositionOld.value());
        PositionOld = positionNew.pos;
        dVelocity = velocity + Acceleration * dt * dt;
        positionNew.pos += dVelocity;
        Acceleration = {0, 0, 0};
    }

    void Verlet::UpdateMovementPosition(float dt, Position &positionNew) {
        if (!PositionOld) {
            PositionOld = positionNew.pos;
            return;
        }
        PositionOld = positionNew.pos;
        dVelocity = Acceleration * dt * dt;
        positionNew.pos += dVelocity;
        Acceleration = {0, 0, 0};
    }


    void Verlet::Accelerate(const glm::vec3 &acceleration) {
        Acceleration += acceleration;
    }
}// namespace voxie