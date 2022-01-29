//
// Created by Stefan Annell on 2022-01-11.
//

#pragma once
#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Delegate.h"

namespace voxie {

struct Position {
    Position(const glm::vec3 &position);
    Position(float x, float y, float z);

    Position(const Position &p);

    bool operator<(const Position &pos) const;

    void encode(YAML::Node &node) const;
    bool decode(const YAML::Node &node);

    void SetPosition(const glm::vec3 &pos);
    void SetScale(const glm::vec3 &scale);

    void SetRotation(const glm::vec3 &rotation);
    void SetModel(const glm::mat4 &model);
    glm::mat4 GetModel(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation);
    void UpdateModel();

    glm::mat4 model{};

    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::quat rotationQuat;

    using OnUpdate = Delegate<>;
    OnUpdate onUpdate;
};

}