//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

namespace YAML {
    class Node;
}

namespace voxie {

    struct Material {
        glm::vec3 color;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;

        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);
    };

}