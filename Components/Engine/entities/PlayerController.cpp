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

        COMPONENT_REGISTER(RigidBody, std::make_shared<RigidBody>(*position.get()));
        COMPONENT_REGISTER(Position, position);
        COMPONENT_REGISTER(Name, name);

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
                                                    auto gameMode = engine.GetGameMode();
                                                    if (gameMode->IsStarted() && !voxie::MouseHandler::IsCameraLocked()) {
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
                                                    if (gameMode->IsStarted() && !voxie::MouseHandler::IsCameraLocked()) {
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
                                                    if (gameMode->IsStarted() && !voxie::MouseHandler::IsCameraLocked()) {
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
                                                    if (gameMode->IsStarted() && !voxie::MouseHandler::IsCameraLocked()) {
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

        voxie::KeyboardHandler::RegisterAction({[this, &engine]() {
                                                    auto gameMode = engine.GetGameMode();
                                                    if (gameMode->IsStarted() && !voxie::MouseHandler::IsCameraLocked()) {
                                                        if (auto body = this->GetRigidBody()) {
                                                            body->ResetForces();
                                                            body->ApplyForceAtCenterOfMass({0, this->jumpHeight, 0});
                                                            this->jumped = true;
                                                        } else {
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
        auto gameMode = Engine::GetEngine().GetGameMode();
        if (!gameMode->IsStarted()) {
            return;
        }

        static float timer = 0;
        timer += delta;
        static float delay = 10;
        static float jumpTimeoutDelay = 1;

        ImGui::SliderFloat("Delay", &delay, 0.1f, 20.0f);
        ImGui::SliderFloat("Jump height", &jumpHeight, 0.1f, 500.0f);
        ImGui::SliderFloat("Jump timeout", &jumpTimeoutDelay, 0.1f, 10.0f);

        if (auto rigidBody = GetRigidBody()) {
            auto linearDampening = rigidBody->GetLinearDampening();
            ImGui::DragFloat("Linear dampening", &linearDampening);
            rigidBody->SetLinearDampening(linearDampening);

            auto bounciness = rigidBody->GetBounciness();
            ImGui::SliderFloat("Bounciness", &bounciness, 0.0f, 1.0f);
            rigidBody->SetBounciness(bounciness);
        }
        if (timer >= delay) {
            timer = 0;
            auto cube = voxie::MakePrimitive({"Cube", voxie::BasePrimitives::Cube});
            auto cubePtr = cube.get();
            voxie::Engine::GetEngine().GetScene()->AddNode(std::move(cube), nullptr);
            auto material = cubePtr->GetMaterial();
            auto makeRandomColor = []() {
                float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                return glm::vec3{r, g, b};
            };
            material->ambient = makeRandomColor();
            material->diffuse = makeRandomColor();
            material->specular = makeRandomColor();
            material->shininess = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100;
            cubePtr->GetPosition()->SetPosition({0, 15, 0});
            cubePtr->GetRigidBody()->SetPosition({0, 15, 0});
            cubePtr->GetRigidBody()->SetBodyType(BodyType::DYNAMIC);
            cubePtr->Init();
        }
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
                CameraPos->SetModel(GetPosition()->model);
            });
        }
    }

    std::shared_ptr<Camera> PlayerController::GetCamera() {
        return helper::GetSceneNode<Camera>(CurrentView);
    }

}// namespace voxie
