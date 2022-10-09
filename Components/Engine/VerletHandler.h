#pragma once
#include <glm/glm.hpp>

namespace voxie {
    class VerletHandler {
    public:
        void Initialize();

    private:
        void Tick(float timeStep) const;
        void UpdateGravity(struct Verlet &) const;
        void ApplyConstraints(Verlet &, struct Position &pos) const;

        glm::vec3 Gravity;
    };
}// namespace voxie