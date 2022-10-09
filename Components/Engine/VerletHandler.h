#pragma once
#include <glm/glm.hpp>

namespace voxie {
    class VerletHandler {
    public:
        void Initialize();

    private:
        void Tick(float timeStep) const;
        void UpdateGravity(struct Verlet &) const;
        void UpdateMovement(struct Verlet &) const;
        void ApplyConstraints(struct Position &pos) const;
        void SolvePointCubeCollisions(struct Position &pos, const Verlet &, const class Handle &) const;

        glm::vec3 Gravity;
    };
}// namespace voxie