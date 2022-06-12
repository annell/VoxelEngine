//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once

#include <string>

namespace YAML {
    class Node;
}

namespace voxie {
    struct Name {
        Name(std::string name);
        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);

        std::string name;
    };
}// namespace voxie
