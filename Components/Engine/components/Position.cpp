//
// Created by Stefan Annell on 2022-01-11.
//

#include "Position.h"
#include <Core.h>

namespace voxie {
    Position::Position() : Position(glm::vec3{}) {
    }

    Position::Position(const glm::vec3 &position)
        : pos(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), rotationQuat(glm::quat(glm::radians(rotation))) {
        SetPosition(position);
        UpdateModel();
    }

    bool Position::operator<(const Position &pos) const {
        return std::tie(pos.pos[0], pos.pos[1], pos.pos[2]) < std::tie(pos.pos[0], pos.pos[1], pos.pos[2]);
    }

    void Position::SetPosition(const glm::vec3 &pos) {
        if (this->pos != pos) {
            this->pos = pos;
        }
    }

    void Position::SetScale(const glm::vec3 &scale) {
        if (this->scale != scale) {
            this->scale = scale;
            if (this->scale.x <= 0.001) {
                this->scale.x = 1;
            }
            if (this->scale.y <= 0.001) {
                this->scale.y = 1;
            }
            if (this->scale.z <= 0.001) {
                this->scale.z = 1;
            }
        }
    }

    void Position::SetRotation(const glm::vec3 &rotation) {
        if (this->rotation != rotation) {
            this->rotation = rotation;
            rotationQuat = glm::quat(glm::radians(rotation));
        }
    }

    void Position::SetModel(const glm::mat4 &model) {
        this->model = model;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(model, scale, rotationQuat, pos, skew, perspective);
        rotation = glm::eulerAngles(rotationQuat);
        onUpdate.Broadcast();
    }

    glm::mat4 Position::GetModel(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rotation) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::scale(model, scale);
        model *= glm::toMat4(glm::quat((rotation)));
        return model;
    }

    void Position::UpdateModel() {
        model = GetModel(pos, scale, rotation);
    }

}// namespace voxie
