//
// Created by Stefan Annell on 2021-05-22.
//
#include "Camera.h"
#include <Engine.h>
#include <memory>

namespace voxie {

    Camera::Camera(Entity entity, Name name, glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : Front(glm::vec3(0.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), WorldUp(up), entity(entity) {
        helper::AddComponent(entity, std::make_shared<Name>(name));
        helper::AddComponent(entity, std::make_shared<Position>(position));
        helper::AddComponent(entity, std::make_shared<Direction>(yaw, pitch));
        updateCameraVectors();
    }

    Camera::~Camera() {
        helper::RemoveComponent<Name>(entity);
        helper::RemoveComponent<Position>(entity);
        helper::RemoveComponent<Direction>(entity);
    }

    void Camera::encode(YAML::Node& node) const {
        node["type"] = "Camera";
        node["name"] = helper::GetComponent<Name>(entity).get()->name;

        node["position"] = *helper::GetComponent<Position>(entity).get();
        node["direction"] = *helper::GetComponent<Direction>(entity).get();
    }

    bool Camera::decode(const YAML::Node& node) {
        GetPosition()->decode(node["position"]);
        GetDirection()->decode(node["direction"]);

        updateCameraVectors();
        return true;
    }

    glm::mat4 Camera::GetViewMatrix() const {
        return glm::lookAt(GetPosition()->pos, GetPosition()->pos + Front, Up);
    }

    glm::mat4 Camera::GetProjectionMatrix() const {
        return glm::perspective(
                glm::radians(Zoom),
                (float) Engine::GetEngine().GetWindow()->GetWidth() / (float) Engine::GetEngine().GetWindow()->GetHeight(),
                1.0f,
                GetFarDistance());
    }

    void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        auto &Position = GetPosition()->pos;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        const auto &direction = GetDirection();
        direction->SetDirection(direction->yaw + xoffset, direction->pitch + yoffset, constrainPitch);

        updateCameraVectors();
    }

    void Camera::SetShaderParameters(const Shader &shader) const {
        shader.setVec3("viewPos", GetPosition()->pos);
        shader.setMat4("projection", GetProjectionMatrix());
        shader.setMat4("view", GetViewMatrix());
    }

    void Camera::updateCameraVectors() {
        glm::vec3 front;
        const auto &direction = GetDirection();
        front.x = cos(glm::radians(direction->yaw)) * cos(glm::radians(direction->pitch));
        front.y = sin(glm::radians(direction->pitch));
        front.z = sin(glm::radians(direction->yaw)) * cos(glm::radians(direction->pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }

    const Entity &Camera::GetEntity() const {
        return entity;
    }

    std::shared_ptr<Position> Camera::GetPosition() const {
        return helper::GetComponent<Position>(entity);
    }

    std::shared_ptr<Direction> Camera::GetDirection() const {
        return helper::GetComponent<Direction>(entity);
    }

    float Camera::GetFarDistance() {
        return 10000.0f;
    }


}// namespace voxie
