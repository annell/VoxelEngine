//
// Created by Stefan Annell on 2022-01-11.
//

#include "Color.h"

namespace voxie {

    Color::Color(glm::vec3 color)
        : color(color) {

    }

    void Color::encode(YAML::Node &node) const {
        node["r"] = color[0];
        node["g"] = color[1];
        node["b"] = color[2];
    }

    bool Color::decode(const YAML::Node &node) {
        color[0] = node["r"].as<float>();
        color[1] = node["g"].as<float>();
        color[2] = node["b"].as<float>();
        return true;
    }

    void Color::SetColor(float r, float g, float b) {
        color = {r, g, b};
    }

}