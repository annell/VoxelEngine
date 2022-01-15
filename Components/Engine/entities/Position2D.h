//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include "Core.h"
#include <yaml-cpp/yaml.h>

namespace voxie {

struct Position2D {
    Position2D(const glm::vec2 &position);
    Position2D(float x, float y);
    Position2D(const Position2D &p);

    void encode(YAML::Node &node) const;
    bool decode(const YAML::Node &node);

    bool operator<(const Position2D &pos) const;

    void SetPosition(float x, float y);
    void SetPosition(const glm::vec2 &pos);
    void SetScale(const glm::vec2 &scale);
    void SetRotation(float rotation);
    void UpdateModel();

    glm::mat4 model{};

    glm::vec2 pos;
    float rotation;
    glm::vec2 scale;
};

}
