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

    COMPONENT_ADD_FUNCTION(Position);
    COMPONENT_ADD_FUNCTION(Name);
};

}
