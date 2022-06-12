//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once

#include <yaml-cpp/yaml.h>

namespace voxie {

    struct Attenuation {
        Attenuation(float constant, float linear, float quadratic);

        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);

        float constant = 0;
        float linear = 0;
        float quadratic = 0;
    };

}// namespace voxie
