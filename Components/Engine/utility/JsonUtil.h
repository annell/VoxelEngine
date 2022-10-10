#pragma once
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace voxie {
    glm::vec3 GetDefaultVec3(const YAML::Node &node, std::string key) {
        return node.IsDefined() && node[key].IsDefined() ? glm::vec3{node["x"].as<float>(0.0f),
                                                                     node["y"].as<float>(0.0f),
                                                                     node["z"].as<float>(0.0f)}
                                                         : glm::vec3{0, 0, 0};
    }
}// namespace voxie

namespace YAML {
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3 &rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node &node, glm::vec3 &rhs) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };
}// namespace YAML
