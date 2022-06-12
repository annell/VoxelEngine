//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include "Delegate.h"
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace voxie {

    struct Color {
        Color(glm::vec3 color);

        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);

        void SetColor(float r, float g, float b);

        glm::vec3 color;

        using OnUpdate = Delegate<>;
        OnUpdate onUpdate;
    };
}// namespace voxie
