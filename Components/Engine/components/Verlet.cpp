#include "Verlet.h"
#include "JsonUtil.h"
#include "Position.h"

namespace voxie {

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