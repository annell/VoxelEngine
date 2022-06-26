#include "PlayerController.h"
#include "Camera.h"
#include "Core.h"
#include "GameMode.h"
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


        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    auto gameMode = engine.GetGameMode();
                                                    if (gameMode->IsStarted()) {
                                                        auto camera = engine.GetCamera();
                                                        auto front = camera->GetFront();
                                                        front.y = 0;
                                                        float velocity = 5 * engine.GetDeltaTime();
                                                        if (auto position = GetPosition()) {
                                                            position->pos += front * velocity;
                                                            position->UpdateModel();
                                                            position->SetModel(position->model);
                                                            GetRigidBody()->SetPosition(*position);
                                                        }
                                                    }
                                                },
                                                voxie::Key::KEY_W});

        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    auto gameMode = engine.GetGameMode();
                                                    if (gameMode->IsStarted()) {
                                                        auto camera = engine.GetCamera();
                                                        auto right = camera->GetRight();
                                                        right.y = 0;
                                                        float velocity = 5 * engine.GetDeltaTime();
                                                        if (auto position = GetPosition()) {
                                                            position->pos -= right * velocity;
                                                            position->UpdateModel();
                                                            position->SetModel(position->model);
                                                            GetRigidBody()->SetPosition(*position);
                                                        }
                                                    }
                                                },
                                                voxie::Key::KEY_A});

        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    auto gameMode = engine.GetGameMode();
                                                    if (gameMode->IsStarted()) {
                                                        auto camera = engine.GetCamera();
                                                        auto front = camera->GetFront();
                                                        front.y = 0;
                                                        float velocity = 5 * engine.GetDeltaTime();
                                                        if (auto position = GetPosition()) {
                                                            position->pos -= front * velocity;
                                                            position->UpdateModel();
                                                            position->SetModel(position->model);
                                                            GetRigidBody()->SetPosition(*position);
                                                        }
                                                    }
                                                },
                                                voxie::Key::KEY_S});

        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    auto gameMode = engine.GetGameMode();
                                                    if (gameMode->IsStarted()) {
                                                        auto camera = engine.GetCamera();
                                                        auto right = camera->GetRight();
                                                        right.y = 0;
                                                        float velocity = 5 * engine.GetDeltaTime();
                                                        if (auto position = GetPosition()) {
                                                            position->pos += right * velocity;
                                                            position->UpdateModel();
                                                            position->SetModel(position->model);
                                                            GetRigidBody()->SetPosition(*position);
                                                        }
                                                    }
                                                },
                                                voxie::Key::KEY_D});
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
        auto pos = GetPosition();
        pos->scale.y = 4;
        pos->scale.x = 2;
        pos->scale.z = 2;
        rigidBody->collider = CreateBoxCollider(rigidBody->rigidBody, *pos);
        rigidBody->SetPosition(*pos);
        rigidBody->decode(node["rigidBody"]);
        SetCamera(Handle(node["cameraView"].as<u_int64_t>()));

        return true;
    }

    void PlayerController::OnTick(float delta) {
    }

    void PlayerController::BeginPlay() {
        voxie::MouseHandler::LockCamera();
    }

    void PlayerController::StopPlay() {
        voxie::MouseHandler::UnlockCamera();
    }

    void PlayerController::SetCamera(const Handle &currentView) {
        CurrentView = currentView;
        if (auto camera = GetCamera()) {
            GetPosition()->onUpdate.Bind([&, CameraPos = camera->GetPosition()]() {
                CameraPos->SetModel(GetPosition()->model);
            });
        }
    }

    std::shared_ptr<Camera> PlayerController::GetCamera() {
        return helper::GetSceneNode<Camera>(CurrentView);
    }
}// namespace voxie
