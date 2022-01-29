//
// Created by Stefan Annell on 2021-10-31.
//

#include "TransformNode.h"
#include "Core.h"

namespace voxie {

TransformNode::TransformNode(const voxie::Handle & handle, std::shared_ptr<Name> name, std::shared_ptr<Position> position)
    : NodeWrapper(handle) {
    COMPONENT_REGISTER(Position, position);
    COMPONENT_REGISTER(Name, name);
}

void TransformNode::encode(YAML::Node& node) const {
    node["type"] = "TransformNode";
    node["id"] = GetHandle().GetId();
    auto name = helper::GetComponent<Name>(handle).get();
    node["name"] = name->name;
    node["position"] = *helper::GetComponent<Position>(handle).get();
}

    bool TransformNode::decode(const YAML::Node& node) {
    GetPosition()->decode(node["position"]);
    return true;
}

}
