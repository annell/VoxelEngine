//
// Created by Stefan Annell on 2022-01-11.
//

#include "Position2D.h"

namespace voxie {

    Position2D::Position2D() : Position2D(glm::vec2{0, 0}) {
    }


    Position2D::Position2D(const glm::vec2 &position)
        : pos(glm::vec2(1.0f)), rotation(0.0), scale(glm::vec2(1.0f)) {
        SetPosition(position);
        UpdateModel();
    }

    bool Position2D::operator<(const Position2D &pos) const {
        return std::tie(pos.pos[0], pos.pos[1]) < std::tie(pos.pos[0], pos.pos[1]);
    }

    void Position2D::SetPosition(float x, float y) {
        SetPosition({x, y});
    }

    void Position2D::SetPosition(const glm::vec2 &pos) {
        if (this->pos != pos) {
            this->pos = pos;
        }
    }

    void Position2D::SetScale(const glm::vec2 &scale) {
        if (this->scale != scale) {
            this->scale = scale;
            if (this->scale.x <= 0.001) {
                this->scale.x = 1;
            }
            if (this->scale.y <= 0.001) {
                this->scale.y = 1;
            }
        }
    }

    void Position2D::SetRotation(float rotation) {
        if (this->rotation != rotation) {
            this->rotation = rotation;
        }
    }

    void Position2D::UpdateModel() {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(pos, 0.0f));

        model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));

        model = glm::scale(model, glm::vec3(scale, 1.0f));
    }

}// namespace voxie