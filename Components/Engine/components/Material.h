//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML {
    class Node;
}

namespace voxie {

    struct Material {
        Material() {}
        Material(const glm::vec3 &, const glm::vec3 &, const glm::vec3 &, float);
        Material(const Material &);
        glm::vec3 ambient = {};
        glm::vec3 diffuse = {};
        glm::vec3 specular = {};
        float shininess = 0;

        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);
    };

}// namespace voxie