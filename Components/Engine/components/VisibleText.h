//
// Created by Stefan Annell on 2022-01-23.
//


#pragma once

#include <string>

namespace YAML {
    class Node;
}

namespace voxie {
    struct VisibleText {
        VisibleText(std::string name);
        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);

        std::string text;
    };
}
