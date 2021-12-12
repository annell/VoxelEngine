//
// Created by Stefan Annell on 2021-05-17.
//

#pragma once
#include "Core.h"
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <yaml-cpp/yaml.h>

namespace voxie {

    struct Name {
        Name(std::string name)
            : name(std::move(name)) {
        }

        void encode(YAML::Node &node) const {
            node["name"] = name;
        }

        bool decode(const YAML::Node &node) {
            if (!node.IsSequence() || node.size() != 1) {
                return false;
            }
            name = node[0].as<std::string>();
            return true;
        }

        std::string name;
    };

    struct Material {
        glm::vec3 color;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;

        void encode(YAML::Node &node) const {
            YAML::Node colorNode;
            colorNode["r"] = color[0];
            colorNode["g"] = color[1];
            colorNode["b"] = color[2];
            node["color"] = colorNode;

            YAML::Node ambientNode;
            ambientNode["x"] = ambient[0];
            ambientNode["y"] = ambient[1];
            ambientNode["z"] = ambient[2];
            node["ambient"] = ambientNode;

            YAML::Node diffuseNode;
            diffuseNode["x"] = diffuse[0];
            diffuseNode["y"] = diffuse[1];
            diffuseNode["z"] = diffuse[2];
            node["diffuse"] = ambientNode;

            YAML::Node specularNode;
            specularNode["x"] = specular[0];
            specularNode["y"] = specular[1];
            specularNode["z"] = specular[2];
            node["specular"] = specularNode;

            node["shininess"] = shininess;
        }

        bool decode(const YAML::Node &node) {
            auto root = node["color"];
            color = {root["r"].as<float>(),
                     root["g"].as<float>(),
                     root["b"].as<float>()};

            root = node["ambient"];
            ambient = {root["x"].as<float>(),
                       root["y"].as<float>(),
                       root["z"].as<float>()};

            root = node["diffuse"];
            diffuse = {root["x"].as<float>(),
                       root["y"].as<float>(),
                       root["z"].as<float>()};

            root = node["specular"];
            diffuse = {root["x"].as<float>(),
                       root["y"].as<float>(),
                       root["z"].as<float>()};

            shininess = node["shininess"].as<float>();

            return true;
        }
    };

    struct Color {
        Color(glm::vec3 color) : color(color) {}

        void encode(YAML::Node &node) const {
            node["r"] = color[0];
            node["g"] = color[1];
            node["b"] = color[2];
        }

        bool decode(const YAML::Node &node) {
            color[0] = node["r"].as<float>();
            color[1] = node["g"].as<float>();
            color[2] = node["b"].as<float>();
            return true;
        }

        void SetColor(float r, float g, float b) {
            color = {r, g, b};
        }

        glm::vec3 color;
    };

    struct Attenuation {
        Attenuation(float constant, float linear, float quadratic)
            : constant(constant), linear(linear), quadratic(quadratic) {
        }

        void encode(YAML::Node &node) const {
            node["constant"] = constant;
            node["linear"] = linear;
            node["quadratic"] = quadratic;
        }

        bool decode(const YAML::Node &node) {
            constant = node["constant"].as<float>();
            linear = node["linear"].as<float>();
            quadratic = node["quadratic"].as<float>();
            return true;
        }

        float constant = 0;
        float linear = 0;
        float quadratic = 0;
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

        void encode(YAML::Node &node) const {
            YAML::Node posNode;
            posNode["x"] = pos[0];
            posNode["y"] = pos[1];
            posNode["z"] = pos[2];
            node["position"] = posNode;

            YAML::Node rotationNode;
            rotationNode["x"] = rotation[0];
            rotationNode["y"] = rotation[1];
            rotationNode["z"] = rotation[2];
            node["rotation"] = rotationNode;

            YAML::Node scaleNode;
            scaleNode["x"] = scale[0];
            scaleNode["y"] = scale[1];
            scaleNode["z"] = scale[2];
            node["scale"] = scaleNode;
        }

        bool decode(const YAML::Node &node) {
            auto rot = node["rotation"];
            SetRotation({rot["x"].as<float>(),
                         rot["y"].as<float>(),
                         rot["z"].as<float>()});

            auto scale = node["scale"];
            SetScale({scale["x"].as<float>(),
                      scale["y"].as<float>(),
                      scale["z"].as<float>()});

            auto pos = node["position"];
            SetPosition({pos["x"].as<float>(),
                         pos["y"].as<float>(),
                         pos["z"].as<float>()});

            UpdateModel();
            return true;
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

        glm::mat4 GetModel(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation) {
            auto model = glm::mat4(1.0f);
            model = glm::translate(glm::mat4(1.0f), pos);
            model = glm::scale(model, scale);
            model *= glm::toMat4(glm::quat((rotation)));
            return model;
        }

        void UpdateModel() {
            model = GetModel(pos, scale, rotation);
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

        void encode(YAML::Node &node) const {
            YAML::Node posNode;
            posNode["x"] = pos[0];
            posNode["y"] = pos[1];
            node["position"] = posNode;

            node["rotation"] = rotation;

            YAML::Node scaleNode;
            scaleNode["x"] = scale[0];
            scaleNode["y"] = scale[1];
            node["scale"] = scaleNode;
        }

        bool decode(const YAML::Node &node) {
            auto pos = node["position"];
            SetPosition({pos["x"].as<float>(),
                         pos["y"].as<float>()});

            SetRotation(node["rotation"].as<float>());

            auto scale = node["scale"];
            SetScale({scale["x"].as<float>(),
                      scale["y"].as<float>()});
            UpdateModel();
            return true;
        }

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

        void encode(YAML::Node &node) const {
            node["yaw"] = yaw;
            node["pitch"] = pitch;
        }

        bool decode(const YAML::Node &node) {
            yaw = node["yaw"].as<float>();
            pitch = node["pitch"].as<float>();
            return true;
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

namespace YAML {
    template<typename T>
    struct convert {
        static Node encode(const T &rhs) {
            Node node;
            rhs.encode(node);
            return node;
        }

        static bool decode(const Node &node, T &rhs) {
            return rhs.decode(node);
        }
    };
}// namespace YAML
