#include "VerletHandler.h"
#include "Core.h"
#include "GameMode.h"

namespace {
    bool IsPointInCube(const voxie::Position &point, const voxie::Position &cube) {
        auto axisInCube = [](float projectionI, float axisI, float length) {
            return projectionI <= axisI + length / 2 && projectionI >= axisI - length / 2;
            //return 2 * abs(projectionI * axisI) <= length;
        };

        auto projection = point.pos;
        //auto projection = point.pos - cube.pos;
        bool xAxis = axisInCube(projection.x, cube.pos.x, cube.scale.x);
        bool yAxis = axisInCube(projection.y, cube.pos.y, cube.scale.y);
        bool zAxis = axisInCube(projection.z, cube.pos.z, cube.scale.z);

        return xAxis && yAxis && zAxis;
    }

}// namespace

namespace voxie {
    void VerletHandler::Initialize() {
        Gravity = {0, -9.81, 0};
        Engine::GetEngine().onTick.Bind(std::bind(&VerletHandler::Tick, this, std::placeholders::_1));
    }

    void VerletHandler::Tick(float timeStep) const {
        ImGui::SliderFloat3("Gravity", (float *) glm::value_ptr(Gravity), -10, 10.0f);
        static const float timeStepFraction = 1.0f / 60.0f;
        static float accumulator = 0.0f;
        accumulator += timeStep;

        if (accumulator >= timeStepFraction) {
            accumulator -= timeStepFraction;
            for (const auto &node : Engine::GetEngine().GetScene()->GetNodesPtrs()) {
                const auto &handle = node->GetHandle();
                if (Engine::GetEngine().GetGameMode()->IsStarted() && helper::HasComponent<Verlet>(handle) && helper::GetComponent<Position>(handle)) {
                    auto verlet = helper::GetComponent<Verlet>(handle);
                    UpdateGravity(*verlet.get());

                    auto pos = helper::GetComponent<Position>(handle);
                    verlet->UpdatePosition(timeStep, *pos.get());
                    ApplyConstraints(*pos.get());
                    SolvePointCubeCollisions(*pos.get(), *verlet.get(), handle);

                    UpdateMovement(*verlet.get());
                    verlet->UpdatePosition(timeStep, *pos.get());
                    ApplyConstraints(*pos.get());
                    SolvePointCubeCollisions(*pos.get(), *verlet.get(), handle);

                    pos->UpdateModel();
                    pos->onUpdate.Broadcast();
                }
            }
        }
    }

    void VerletHandler::UpdateGravity(Verlet &verlet) const {
        verlet.Accelerate(Gravity);
    }

    void VerletHandler::UpdateMovement(Verlet &verlet) const {
        verlet.Accelerate(verlet.MovementVelocity);
        verlet.MovementVelocity = {0, 0, 0};
    }

    void VerletHandler::ApplyConstraints(Position &pos) const {
        if (pos.pos.y < -1000 || pos.pos.y > 1000) {
            pos.pos.y = 10;
        }
    }

    void VerletHandler::SolvePointCubeCollisions(Position &point, const Verlet &verlet, const Handle &handle) const {
        for (const auto &node : Engine::GetEngine().GetScene()->GetNodesPtrs()) {
            const auto &targetHandle = node->GetHandle();
            if (targetHandle == handle) {
                continue;
            }
            if (auto entity = std::dynamic_pointer_cast<voxie::CubeEntity>(node)) {
                if (auto cube = entity->GetPosition()) {
                    if (IsPointInCube(point, *cube.get())) {
                        point.pos -= verlet.dVelocity;
                    }
                }
            }
        }
    }

}// namespace voxie
