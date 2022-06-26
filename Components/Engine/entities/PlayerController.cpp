#include "PlayerController.h"
#include "Core.h"
#include "Name.h"
#include "Position.h"
#include "RigidBody.h"

namespace voxie {
    PlayerController::PlayerController(const Handle &handle, std::shared_ptr<Name> name, std::shared_ptr<Position> position)
        : NodeWrapper(handle), CurrentView(0) {
        auto &engine = Engine::GetEngine();
        OnTickHandle = engine.onTick.Bind(std::bind(&PlayerController::OnTick, this, std::placeholders::_1));

        //COMPONENT_REGISTER(RigidBody, std::make_shared<RigidBody>(*position.get()));
        COMPONENT_REGISTER(Position, position);
        COMPONENT_REGISTER(Name, name);
    }

    void PlayerController::OnTick(float delta) {
    }

    void PlayerController::SetCamera(const Handle & currentView) {
        CurrentView = currentView;
    }
}// namespace voxie
