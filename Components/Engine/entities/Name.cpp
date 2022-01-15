//
// Created by Stefan Annell on 2022-01-11.
//

#include "Name.h"

#include <yaml-cpp/yaml.h>

namespace voxie {
    Name::Name(std::string name)
            : name(std::move(name)) {
    }

    void Name::encode(YAML::Node &node) const {
        node["name"] = name;
    }

    bool Name::decode(const YAML::Node &node) {
        if (!node.IsSequence() || node.size() != 1) {
            return false;
        }
        name = node[0].as<std::string>();
        return true;
    }
}
