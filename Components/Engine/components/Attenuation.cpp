//
// Created by Stefan Annell on 2022-01-11.
//

#include "Attenuation.h"

namespace voxie {

    Attenuation::Attenuation(float constant, float linear, float quadratic)
        : constant(constant), linear(linear), quadratic(quadratic) {
    }

    void Attenuation::encode(YAML::Node &node) const {
        node["constant"] = constant;
        node["linear"] = linear;
        node["quadratic"] = quadratic;
    }

    bool Attenuation::decode(const YAML::Node &node) {
        constant = node["constant"].as<float>();
        linear = node["linear"].as<float>();
        quadratic = node["quadratic"].as<float>();
        return true;
    }
}// namespace voxie
