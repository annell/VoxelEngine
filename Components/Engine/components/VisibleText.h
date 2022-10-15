//
// Created by Stefan Annell on 2022-01-23.
//


#pragma once

#include "JsonUtil.h"
#include <string>

namespace voxie {
    struct VisibleText {
        VisibleText();
        VisibleText(std::string name);

        std::string text;
    };
}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::VisibleText> {
        static Node encode(const voxie::VisibleText &rhs) {
            Node node;
            node["text"] = rhs.text;
            return node;
        }

        static bool decode(const Node &node, voxie::VisibleText &rhs) {
            if (!node["text"].IsDefined()) {
                return false;
            }
            rhs.text = node["text"].as<std::string>("");
            return true;
        }
    };
}// namespace YAML