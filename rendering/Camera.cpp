//
// Created by Stefan Annell on 2021-05-22.
//
#include "Camera.h"
#include <memory>
#include <Engine.h>

namespace voxie {

Camera::Camera(std::shared_ptr<Entity> entity, glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: Front(glm::vec3(0.0f, 0.0f, 0.0f))
, MovementSpeed(SPEED)
, MouseSensitivity(SENSITIVITY)
, Zoom(ZOOM)
, WorldUp(up)
, Yaw(yaw)
, Pitch(pitch)
, entity(entity) {
    helper::AddComponent(std::make_shared<Position>(position), *entity);
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(GetPosition()->pos, GetPosition()->pos + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    auto& Position = GetPosition()->pos;
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

    Yaw   += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 90.0f)
        Zoom = 90.0f;
}

void Camera::SetShaderParameters(const Shader& shader) const {
    glm::mat4 projection = glm::perspective(glm::radians(Zoom), (float)800 / (float)600, 0.1f, 100000.0f);
    glm::mat4 view = GetViewMatrix();

    shader.setVec3("viewPos", GetPosition()->pos);
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

std::shared_ptr<Entity> Camera::GetEntity() const {
    return entity;
}

std::shared_ptr<Position> Camera::GetPosition() const {
    return helper::GetComponent<Position>(*entity);
}


}
