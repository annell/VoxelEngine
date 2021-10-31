//
// Created by Stefan Annell on 2021-10-31.
//

#pragma once

#include "Core.h"
#include "EntityComponentSystem.h"
#include <Node.h>

namespace voxie {

class TransformNode : public NodeWrapper {
public:
    TransformNode(const Entity&, std::shared_ptr<Name>, std::shared_ptr<Position>);

    void encode(YAML::Node &) const;
    bool decode(const YAML::Node &);

    const Entity& GetEntity() const;

    [[nodiscard]] std::shared_ptr<Position> GetPosition() const;
    [[nodiscard]] std::shared_ptr<Name> GetName() const;
private:
    Entity entity;
};

}
