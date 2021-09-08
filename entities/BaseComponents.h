//
// Created by Stefan Annell on 2021-05-17.
//

#pragma once
#include "Core.h"
#include <string>
#include <tuple>
#include <utility>


namespace voxie {

    struct Name {
        Name(std::string name)
            : name(std::move(name)) {
        }

        std::string name;
    };

    struct Material {
        glm::vec3 color;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };

    struct Color {
        Color(glm::vec3 color) : color(color) {}
        glm::vec3 color;

        void SetColor(float r, float g, float b) {
            color = {r, g, b};
        }
    };

    struct Position {
        Position(const glm::vec3 &position)
            : pos(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), rotationQuat(glm::quat(glm::radians(rotation))) {
            SetPosition(position);
            UpdateModel();
        }

        Position(float x, float y, float z)
            : Position(glm::vec3{x, y, z}) {}

        Position(const Position &p)
            : Position(p.pos) {}

        bool operator<(const Position &pos) const {
            return std::tie(pos.pos[0], pos.pos[1], pos.pos[2]) < std::tie(pos.pos[0], pos.pos[1], pos.pos[2]);
        }

        void SetPosition(float x, float y, float z) {
            SetPosition({x, y, z});
        }

        void SetPosition(const glm::vec3 &pos) {
            if (this->pos != pos) {
                this->pos = pos;
            }
        }

        void SetScale(const glm::vec3 &scale) {
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

        void SetRotation(const glm::vec3 &rotation) {
            if (this->rotation != rotation) {
                this->rotation = rotation;
                rotationQuat = glm::quat(glm::radians(rotation));
            }
        }

        void SetModel(const glm::mat4 &model) {
            this->model = model;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(model, scale, rotationQuat, pos, skew, perspective);
            rotation = glm::eulerAngles(rotationQuat);
        }

        void UpdateModel() {
            model = glm::translate(glm::mat4(1.0f), pos);
            model = glm::scale(model, scale);
            model *= glm::toMat4(rotationQuat);
        }

        glm::mat4 model{};

        glm::vec3 pos;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::quat rotationQuat;
    };

    struct Position2D {
        Position2D(const glm::vec2 &position)
            : pos(glm::vec2(1.0f)), rotation(0.0), scale(glm::vec2(1.0f)) {
            SetPosition(position);
            UpdateModel();
        }

        Position2D(float x, float y)
            : Position2D(glm::vec2{x, y}) {}

        Position2D(const Position2D &p)
            : Position2D(p.pos) {}

        bool operator<(const Position2D &pos) const {
            return std::tie(pos.pos[0], pos.pos[1]) < std::tie(pos.pos[0], pos.pos[1]);
        }

        void SetPosition(float x, float y) {
            SetPosition({x, y});
        }

        void SetPosition(const glm::vec2 &pos) {
            if (this->pos != pos) {
                this->pos = pos;
            }
        }

        void SetScale(const glm::vec2 &scale) {
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

        void SetRotation(float rotation) {
            if (this->rotation != rotation) {
                this->rotation = rotation;
            }
        }

        void UpdateModel() {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(pos, 0.0f));

            model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));

            model = glm::scale(model, glm::vec3(scale, 1.0f));
        }

        glm::mat4 model{};

        glm::vec2 pos;
        float rotation;
        glm::vec2 scale;
    };

    struct Direction {
        Direction(float yaw, float pitch)
            : yaw(yaw), pitch(pitch) {
        }
        void SetDirection(float yaw, float pitch, bool constrainPitch) {
            this->yaw = yaw;
            this->pitch = pitch;

            if (constrainPitch) {
                if (this->pitch > 89.0f)
                    this->pitch = 89.0f;
                if (this->pitch < -89.0f)
                    this->pitch = -89.0f;
            }
        }
        float yaw;
        float pitch;
    };

    struct ChunkPosition {
        int x;
        int y;
        int z;

        bool operator<(const ChunkPosition &pos) const {
            return std::tie(x, y, z) < std::tie(pos.x, pos.y, pos.z);
        }
    };

    struct Dimensions {
        float width;
        float height;
        float depth;
    };

}// namespace voxie
