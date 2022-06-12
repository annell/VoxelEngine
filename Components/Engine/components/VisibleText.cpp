//
// Created by Stefan Annell on 2022-01-23.
//

#include "VisibleText.h"
#include <yaml-cpp/yaml.h>

namespace voxie {
    VisibleText::VisibleText(std::string text)
        : text(text) {
    }

    void VisibleText::encode(YAML::Node &node) const {
        node["text"] = text;
    }

    bool VisibleText::decode(const YAML::Node &node) {
        text = node["text"].as<std::string>();
        return true;
    }
}// namespace voxie
