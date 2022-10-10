#include "Verlet.h"
#include "JsonUtil.h"
#include "Position.h"

namespace voxie {
    void Verlet::encode(YAML::Node &node) const {
        node["directions"] = Directions;
        node["dynamic"] = dynamic;
    }

    bool Verlet::decode(const YAML::Node &node) {
        Directions = node["directions"].as<glm::vec3>(glm::vec3{0, 0, 0});
        dynamic = node["dynamic"].as<bool>(false);
        return true;
    }

    void Verlet::UpdatePosition(float dt, Position &positionNew) {
        if (!PositionOld) {
            PositionOld = positionNew.pos;
        }
        const glm::vec3 velocity = Directions * (positionNew.pos - PositionOld.value());
        PositionOld = positionNew.pos;
        dVelocity = velocity + Acceleration * dt * dt;
        MovementVelocity = Movement * dt * dt;

        positionNew.pos += dVelocity + MovementVelocity;
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