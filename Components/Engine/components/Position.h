//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include "Delegate.h"
#include "JsonUtil.h"
#include <glm/gtx/quaternion.hpp>

namespace voxie {

    struct Position {
        Position();
        Position(const glm::vec3 &position);

        bool operator<(const Position &pos) const;

        void SetPosition(const glm::vec3 &pos);
        void SetScale(const glm::vec3 &scale);

        void SetRotation(const glm::vec3 &rotation);
        void SetModel(const glm::mat4 &model);
        glm::mat4 GetModel(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rotation);
        void UpdateModel();

        glm::mat4 model{};

        glm::vec3 pos{};
        glm::vec3 rotation{};
        glm::vec3 scale{};
        glm::quat rotationQuat;

        using OnUpdate = Delegate<>;
        OnUpdate onUpdate;
    };

}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::Position> {
        static Node encode(const voxie::Position &rhs) {
            Node node;
            node["position"] = rhs.pos;
            node["rotation"] = rhs.rotation;
            node["scale"] = rhs.scale;
            return node;
        }

        static bool decode(const Node &node, voxie::Position &rhs) {
            if (!node["position"].IsDefined() && !node["rotation"].IsDefined() && !node["scale"].IsDefined()) {
                return false;
            }

            rhs.rotation = node["rotation"].as<glm::vec3>(glm::vec3{});
            rhs.scale = node["scale"].as<glm::vec3>(glm::vec3{});
            rhs.pos = node["position"].as<glm::vec3>(glm::vec3{});
            rhs.UpdateModel();
            return true;
        }
    };
}// namespace YAML
