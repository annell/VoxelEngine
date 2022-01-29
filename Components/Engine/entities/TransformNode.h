//
// Created by Stefan Annell on 2021-10-31.
//

#pragma once

#include "Handle.h"
#include <Node.h>

namespace voxie {
    class Position;
    class Name;

class TransformNode : public NodeWrapper {
public:
    TransformNode(const Handle &, std::shared_ptr<Name>, std::shared_ptr<Position>);

    void encode(YAML::Node &) const;
    bool decode(const YAML::Node &);

    const Handle &GetHandle() const override;

    std::shared_ptr<Position> GetPosition() const override;
    std::shared_ptr<Name> GetName() const;
private:
    Handle entity;
};

}
