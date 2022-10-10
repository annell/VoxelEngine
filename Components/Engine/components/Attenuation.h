//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once

#include <yaml-cpp/yaml.h>

namespace voxie {

    struct Attenuation {
        float constant = 0;
        float linear = 0;
        float quadratic = 0;
    };

}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::Attenuation> {
        static Node encode(const voxie::Attenuation &rhs) {
            Node node;
            node["constant"] = rhs.constant;
            node["linear"] = rhs.linear;
            node["quadratic"] = rhs.quadratic;
            return node;
        }

        static bool decode(const Node &node, voxie::Attenuation &rhs) {
            if (!node["constant"].IsDefined() || !node["linear"].IsDefined() || !node["quadratic"].IsDefined()) {
                return false;
            }

            rhs.constant = node["constant"].as<float>(0);
            rhs.linear = node["linear"].as<float>(0);
            rhs.quadratic = node["quadratic"].as<float>(0);
            return true;
        }
    };
}// namespace YAML
