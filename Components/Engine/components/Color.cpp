//
// Created by Stefan Annell on 2022-01-11.
//

#include "Color.h"

namespace voxie {

    Color::Color() : Color({0, 0, 0}) {
    }

    Color::Color(glm::vec3 color)
        : color(color) {
    }

    void Color::SetColor(float r, float g, float b) {
        color = {r, g, b};
        onUpdate.Broadcast();
    }

}// namespace voxie