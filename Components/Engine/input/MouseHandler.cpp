//
// Created by Stefan Annell on 2021-05-09.
//

#include "MouseHandler.h"
#include "Core.h"
#include "EditorGameMode.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace voxie {
    namespace internal {

        double x = 0;
        double y = 0;
        double lastX = 0;
        double lastY = 0;
        bool firstMouse = true;
        bool mouseLock = false;
        bool movementLock = false;
        bool castRay = false;

        MouseHandler::RegistredKeys &GetRegisteredMouseKeys() {
            static MouseHandler::RegistredKeys registeredKeys;

            return registeredKeys;
        }

    }// namespace internal

    void MouseHandler::framebuffer_size_callback(GLFWwindow *, int width, int height) {
        glViewport(0, 0, width, height);
        voxie::Engine::GetEngine().GetWindow()->SetWidth(static_cast<unsigned int>(width));
        voxie::Engine::GetEngine().GetWindow()->SetHeight(static_cast<unsigned int>(height));
    }

    void MouseHandler::mouse_callback(GLFWwindow *, double xpos, double ypos) {
        internal::x = xpos;
        internal::y = ypos;
        if (internal::mouseLock) {
            if (internal::firstMouse) {
                internal::lastX = xpos;
                internal::lastY = ypos;
                internal::firstMouse = false;
            }

            double xoffset = xpos - internal::lastX;
            double yoffset = internal::lastY - ypos;// reversed since y-coordinates go from bottom to top

            internal::lastX = xpos;
            internal::lastY = ypos;

            if (internal::movementLock) {
                if (xoffset > 0) {
                    Engine::GetEngine().GetCamera()->ProcessKeyboard(voxie::RIGHT, 0.1);
                } else if (xoffset < 0) {
                    Engine::GetEngine().GetCamera()->ProcessKeyboard(voxie::LEFT, 0.1);
                }
                if (yoffset > 0) {
                    Engine::GetEngine().GetCamera()->ProcessKeyboard(voxie::FORWARD, 0.1);
                } else if (yoffset < 0) {
                    Engine::GetEngine().GetCamera()->ProcessKeyboard(voxie::BACKWARD, 0.1);
                }
            } else {
                Engine::GetEngine().GetCamera()->ProcessMouseMovement(static_cast<float>(xoffset), static_cast<float>(yoffset));
            }
        }
        if (internal::castRay) {
            internal::castRay = false;
        }
    }

    void MouseHandler::scroll_callback(GLFWwindow *, double, double) {
    }

    void MouseHandler::UnlockCamera() {
        glfwSetInputMode(Engine::GetEngine().GetWindow()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        internal::mouseLock = false;
    }

    void MouseHandler::LockCamera() {
        glfwSetInputMode(Engine::GetEngine().GetWindow()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (!internal::mouseLock) {
            internal::firstMouse = true;
        }
        internal::mouseLock = true;
    }

    void MouseHandler::CastRay() {
        internal::castRay = true;
        Engine::GetEngine().GetPhysicsHandler().RayCast(
                Engine::GetEngine().GetCamera()->GetRay(internal::x, internal::y),
                [](const RaycastInfo &info) {
                    std::cout << "Response from Raycast:" << std::endl;
                    std::cout << "id: " << info.collisionId << std::endl;
                    std::cout << "Collsion Pos: " << info.worldPoint.x << " " << info.worldPoint.y << " " << info.worldPoint.z << std::endl;
                    std::cout << "Collision normal: " << info.worldNormal.x << " " << info.worldNormal.y << " " << info.worldNormal.z << std::endl;
                    for (const auto &entity : Engine::GetEngine().GetScene().GetEntities()) {
                        if (auto rigidBody = helper::GetComponent<RigidBody>(entity)) {
                            if (rigidBody->GetColliderId() == info.collisionId) {
                                std::cout << "Found body, selecting: " << entity.GetId() << std::endl;
                                auto editorGameMode = dynamic_cast<voxie::EditorGameMode *>(voxie::Engine::GetEngine().GetGameMode());
                                editorGameMode->SetSelection(entity);
                                return;
                            }
                        }
                    }
                });
    }

    void MouseHandler::MovementUnlock() {
        internal::movementLock = false;
    }

    void MouseHandler::MovementLock() {
        if (internal::mouseLock) {
            internal::movementLock = true;
        }
    }

    void MouseHandler::processInput() {
        auto &engine = Engine::GetEngine();
        for (auto &key : internal::GetRegisteredMouseKeys()) {
            if (glfwGetMouseButton(engine.GetWindow()->GetWindow(), key.key) == key.type) {
                key.action();
            }
        }
    }

    void MouseHandler::RegisterAction(const MouseAction &keyAction) {
        auto &keys = internal::GetRegisteredMouseKeys();
        keys.push_back(keyAction);
    }

}// namespace voxie
