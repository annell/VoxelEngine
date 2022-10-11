//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include "JsonUtil.h"

namespace YAML {
    class Node;
}

namespace voxie {

    struct Material {
        Material();
        Material(const glm::vec3 &, const glm::vec3 &, const glm::vec3 &, float);
        Material(const Material &);
        glm::vec3 ambient = {};
        glm::vec3 diffuse = {};
        glm::vec3 specular = {};
        float shininess = 0;
    };

}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::Material> {
        static Node encode(const voxie::Material &rhs) {
            Node node;
            node["ambient"] = rhs.ambient;
            node["diffuse"] = rhs.diffuse;
            node["specular"] = rhs.specular;
            node["shininess"] = rhs.shininess;
            return node;
        }

        static bool decode(const Node &node, voxie::Material &rhs) {
            if (!node["ambient"].IsDefined() && !node["diffuse"].IsDefined() && !node["specular"].IsDefined() && !node["shininess"].IsDefined()) {
                return false;
            }

            rhs.ambient = node["ambient"].as<glm::vec3>(glm::vec3{});
            rhs.diffuse = node["diffuse"].as<glm::vec3>(glm::vec3{});
            rhs.specular = node["specular"].as<glm::vec3>(glm::vec3{});
            rhs.shininess = node["shininess"].as<float>(0.0f);
            return true;
        }
    };
}// namespace YAML
