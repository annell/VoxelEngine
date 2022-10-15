//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include "Core.h"
#include "JsonUtil.h"

namespace voxie {

    struct Position2D {
        Position2D();
        Position2D(const glm::vec2 &position);

        bool operator<(const Position2D &pos) const;

        void SetPosition(float x, float y);
        void SetPosition(const glm::vec2 &pos);
        void SetScale(const glm::vec2 &scale);
        void SetRotation(float rotation);
        void UpdateModel();

        glm::mat4 model{};

        glm::vec2 pos;
        float rotation;
        glm::vec2 scale;
    };

}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::Position2D> {
        static Node encode(const voxie::Position2D &rhs) {
            Node node;
            node["position"] = rhs.pos;
            node["rotation"] = rhs.rotation;
            node["scale"] = rhs.scale;
            return node;
        }

        static bool decode(const Node &node, voxie::Position2D &rhs) {
            if (!node["position"].IsDefined() && !node["rotation"].IsDefined() && !node["scale"].IsDefined()) {
                return false;
            }

            rhs.rotation = node["rotation"].as<float>(0.0f);
            rhs.scale = node["scale"].as<glm::vec2>(glm::vec2{});
            rhs.pos = node["position"].as<glm::vec2>(glm::vec2{});
            rhs.UpdateModel();
            return true;
        }
    };
}// namespace YAML
