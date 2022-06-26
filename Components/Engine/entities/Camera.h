//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include "components/Name.h"
#include <EntityComponentSystem.h>
#include <Node.h>
#include <glm/glm.hpp>

namespace voxie {
    class Direction;
    class Shader;
    class Position;

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

    class Camera : public NodeWrapper {
    public:
        Camera(Handle handle, Name name, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        ~Camera();

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        COMPONENT_ADD_FUNCTION(Direction);
        COMPONENT_ADD_FUNCTION(Name);
        COMPONENT_ADD_FUNCTION(Position);

        glm::mat4 GetViewMatrix() const;
        static float GetFarDistance();
        glm::mat4 GetProjectionMatrix() const;
        void ProcessKeyboard(Camera_Movement direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        Ray GetRay(float mouseX, float mouseY);
        void SetShaderParameters(const Shader &shader) const;
        void SetSelection(const Handle &selection);
        const Handle &GetSelection() const;

        const glm::vec3 &GetFront() const;
        const glm::vec3 &GetRight() const;

    private:
        void updateCameraVectors();
        Handle selectedEntity;

        glm::vec3 Front;
        glm::vec3 Up{};
        glm::vec3 Right{};
        glm::vec3 WorldUp;
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
    };

}// namespace voxie
