//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include "BaseComponents.h"
#include "Core.h"
#include "Shader.h"
#include <EntityComponentSystem.h>

namespace voxie {

    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    const float YAW = 45.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;

    class Camera {
    public:
        Camera(Entity entity, Name name, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

        glm::mat4 GetViewMatrix() const;
        float GetFarDistance() const;
        glm::mat4 GetProjectionMatrix() const;
        const Entity &GetEntity() const;
        std::shared_ptr<Direction> GetDirection() const;
        void ProcessKeyboard(Camera_Movement direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
        void SetShaderParameters(const Shader &shader) const;

    private:
        void updateCameraVectors();
        std::shared_ptr<Position> GetPosition() const;
        Entity entity;

        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
    };

}// namespace voxie
