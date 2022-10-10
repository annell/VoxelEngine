#pragma once
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

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

            rhs.x = node[0].as<float>(0);
            rhs.y = node[1].as<float>(0);
            rhs.z = node[2].as<float>(0);
            return true;
        }
    };
}// namespace YAML
