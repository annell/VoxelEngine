#include "PlayerController.h"
#include "Camera.h"
#include "Core.h"
#include "GameMode.h"
#include "Name.h"
#include "Position.h"
#include "RigidBody.h"
#include "Verlet.h"


namespace voxie {
    PlayerController::PlayerController(const Handle &handle, std::shared_ptr<Name> name, std::shared_ptr<Position> position)
        : NodeWrapper(handle), CurrentView(0) {
        auto &engine = Engine::GetEngine();

        COMPONENT_REGISTER(RigidBody, std::make_shared<RigidBody>(*position.get()));
        COMPONENT_REGISTER(Position, position);
        COMPONENT_REGISTER(Name, name);
        COMPONENT_REGISTER(Verlet, std::make_shared<Verlet>());

        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    auto gameMode = engine.GetGameMode();
                                                    if (gameMode->IsStarted()) {
                                                        MouseHandler::LockCamera();
                                                    }
                                                },
                                                voxie::Key::KEY_LEFT_CONTROL});

        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    auto gameMode = engine.GetGameMode();
                                                    if (gameMode->IsStarted()) {
                                                        MouseHandler::UnlockCamera();
                                                    }
                                                },
                                                voxie::Key::KEY_LEFT_ALT});


        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    Movement(engine.GetCamera()->GetFront());
                                                },
                                                voxie::Key::KEY_W});

        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    auto left = glm::vec3{-1, -1, -1} * engine.GetCamera()->GetRight();
                                                    Movement(left);
                                                },
                                                voxie::Key::KEY_A});

        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    auto back = glm::vec3{-1, -1, -1} * engine.GetCamera()->GetFront();
                                                    Movement(back);
                                                },
                                                voxie::Key::KEY_S});

        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    Movement(engine.GetCamera()->GetRight());
                                                },
                                                voxie::Key::KEY_D});

        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    auto gameMode = engine.GetGameMode();
                                                    if (gameMode->IsStarted() && !voxie::MouseHandler::IsCameraLocked() && !this->jumped) {
                                                        if (auto verlet = this->GetVerlet()) {
                                                            if (verlet->OnGround) {
                                                                verlet->Accelerate({0, 300, 0});
                                                                this->jumped = true;
                                                            }
                                                        }
                                                    }
                                                },
                                                voxie::Key::KEY_SPACE});

        voxie::MouseHandler::RegisterAction({[this, &engine]() {
                                                 auto gameMode = engine.GetGameMode();
                                                 if (gameMode->IsStarted() && !voxie::MouseHandler::IsCameraLocked()) {
                                                     auto physicsHandler = engine.GetPhysicsHandler();
                                                     auto window = engine.GetWindow();
                                                     if (auto camera = this->GetCamera()) {
                                                         physicsHandler->RayCast(
                                                                 camera->GetRay(window->GetWidth() / 2, window->GetHeight() / 2),
                                                                 [this, &physicsHandler](const RaycastInfo &info) {
                                                                     auto handle = physicsHandler->GetHandleFromRigidBodyId(info.collisionId);
                                                                     if (auto body = helper::GetComponent<RigidBody>(handle)) {
                                                                         auto myPos = this->GetPosition();
                                                                         auto bodyPos = helper::GetComponent<Position>(handle);
                                                                         auto pos = glm::normalize(bodyPos->pos - myPos->pos);
                                                                         pos *= 1000;
                                                                         body->ApplyForceAtCenterOfMass({pos.x, pos.y, pos.z});
                                                                     }
                                                                 });
                                                     }
                                                 }
                                             },
                                             voxie::MouseButton::BUTTON_1, voxie::ActionType::PRESS});
        OnTickHandle = engine.onTick.Bind(std::bind(&PlayerController::OnTick, this, std::placeholders::_1));
    }
    PlayerController::~PlayerController() {
        KeyboardHandler::Reset();// TODO Fix up so that we can bind keyboard actions to a specific object or each bind returns a handle.
        Engine::GetEngine().onTick.Unbind(OnTickHandle);
    }

    void PlayerController::encode(YAML::Node &node) const {
        node["type"] = "PlayerController";
        node["id"] = GetHandle().GetId();
        auto name = helper::GetComponent<Name>(handle).get();
        node["name"] = name->name;
        node["position"] = *helper::GetComponent<Position>(handle).get();
        node["rigidBody"] = *helper::GetComponent<RigidBody>(handle).get();
        node["cameraView"] = CurrentView.GetId();
        node["verlet"] = *GetVerlet().get();
    }

    bool PlayerController::decode(const YAML::Node &node) {
        DecodeComponent(node["position"], GetPosition());
        auto rigidBody = GetRigidBody();
        auto pos = GetPosition();
        rigidBody->collider = CreateBoxCollider(rigidBody->rigidBody, *pos);
        rigidBody->SetPosition(*pos);
        rigidBody->decode(node["rigidBody"]);
        SetCamera(Handle(node["cameraView"].as<u_int64_t>()));
        DecodeComponent(node["verlet"], GetVerlet());

        return true;
    }

    void PlayerController::OnTick(float delta) {
        auto gameMode = Engine::GetEngine().GetGameMode();
        if (!gameMode->IsStarted()) {
            return;
        }

        static float timer = 0;
        timer += delta;
        static float delay = 10;
        static float jumpTimeoutDelay = 1;

        static float jumpTimeout = 0;
        static bool jumpCountDown = false;
        if (jumped && !jumpCountDown) {
            jumpTimeout = jumpTimeoutDelay;
            jumpCountDown = true;
        }
        if (jumpCountDown) {
            jumpTimeout -= delta;
            if (jumpTimeout <= 0.1) {
                jumped = false;
                jumpCountDown = false;
            }
        }
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
                auto pos = GetPosition();
                CameraPos->pos = pos->pos;
                CameraPos->pos.y += pos->scale.y;
                CameraPos->UpdateModel();
            });
        }
    }

    void PlayerController::Movement(const glm::vec3 &direction) {
        if (Engine::GetEngine().GetGameMode()->IsStarted() && !voxie::MouseHandler::IsCameraLocked()) {
            float velocity = 500;
            GetVerlet()->Movement += glm::normalize(direction * glm::vec3{1, 0, 1}) * velocity;
        }
    }

    std::shared_ptr<Camera> PlayerController::GetCamera() {
        return helper::GetSceneNode<Camera>(CurrentView);
    }

}// namespace voxie
