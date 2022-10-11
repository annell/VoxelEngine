//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once

#include "JsonUtil.h"
#include <string>

namespace voxie {
    struct Name {
        Name();
        Name(std::string name);

        std::string name;
    };
}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::Name> {
        static Node encode(const voxie::Name &rhs) {
            Node node;
            node["name"] = rhs.name;
            return node;
        }

        static bool decode(const Node &node, voxie::Name &rhs) {
            if (!node["name"].IsDefined()) {
                return false;
            }

            rhs.name = node["name"].as<std::string>("");
            return true;
        }
    };
}// namespace YAML
