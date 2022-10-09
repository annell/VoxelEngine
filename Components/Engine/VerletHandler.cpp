#include "VerletHandler.h"
#include "Core.h"
#include "GameMode.h"

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
                if (Engine::GetEngine().GetGameMode()->IsStarted() && helper::HasComponent<Verlet>(node->GetHandle()) && helper::GetComponent<Position>(node->GetHandle())) {
                    auto verlet = helper::GetComponent<Verlet>(node->GetHandle());
                    UpdateGravity(*verlet.get());

                    auto pos = helper::GetComponent<Position>(node->GetHandle());
                    verlet->UpdatePosition(timeStep, *pos.get());
                    ApplyConstraints(*verlet.get(), *pos.get());
                    pos->UpdateModel();
                    pos->onUpdate.Broadcast();
                }
            }
        }
    }

    void VerletHandler::UpdateGravity(Verlet &verlet) const {
        verlet.Accelerate(Gravity);
    }

    void VerletHandler::ApplyConstraints(Verlet &verlet, Position &pos) const {
        if (pos.pos.y < 0) {
            pos.pos.y = 0;
        }
    }

}// namespace voxie