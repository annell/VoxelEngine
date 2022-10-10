//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include "Delegate.h"
#include "JsonUtil.h"

namespace voxie {

    struct Color {
        Color();
        Color(glm::vec3 color);
        void SetColor(float r, float g, float b);

        glm::vec3 color;

        using OnUpdate = Delegate<>;
        OnUpdate onUpdate;
    };
}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::Color> {
        static Node encode(const voxie::Color &rhs) {
            Node node;
            node["color"] = rhs.color;
            return node;
        }

        static bool decode(const Node &node, voxie::Color &rhs) {
            if (!node["color"].IsDefined()) {
                return false;
            }

            rhs.color = node["color"].as<glm::vec3>(glm::vec3{});
            return true;
        }
    };
}// namespace YAML
