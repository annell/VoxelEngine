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

        COMPONENT_REGISTER(RigidBody, std::make_shared<RigidBody>(*position.get()));
        COMPONENT_REGISTER(Position, position);
        COMPONENT_REGISTER(Name, name);
    }

    void PlayerController::encode(YAML::Node &node) const {
        node["type"] = "PlayerController";
        node["id"] = GetHandle().GetId();
        auto name = helper::GetComponent<Name>(handle).get();
        node["name"] = name->name;
        node["position"] = *helper::GetComponent<Position>(handle).get();
        node["rigidBody"] = *helper::GetComponent<RigidBody>(handle).get();
        node["cameraView"] = CurrentView.GetId();
    }

    bool PlayerController::decode(const YAML::Node &node) {
        GetPosition()->decode(node["position"]);
        auto rigidBody = GetRigidBody();
        rigidBody->collider = CreateBoxCollider(rigidBody->rigidBody, *GetPosition());
        rigidBody->SetPosition(*GetPosition());
        rigidBody->decode(node["rigidBody"]);
        CurrentView = Handle(node["cameraView"].as<u_int64_t>());

        return true;
    }

    void PlayerController::OnTick(float delta) {
    }

    void PlayerController::SetCamera(const Handle &currentView) {
        CurrentView = currentView;
    }

    std::shared_ptr<Camera> PlayerController::GetCamera() {
        return helper::GetSceneNode<Camera>(CurrentView);
    }
}// namespace voxie
