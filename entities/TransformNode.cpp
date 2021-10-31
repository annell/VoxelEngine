//
// Created by Stefan Annell on 2021-10-31.
//

#include "TransformNode.h"
#include <Engine.h>

namespace voxie {

TransformNode::TransformNode(const voxie::Entity & entity, std::shared_ptr<Name> name, std::shared_ptr<Position> position)
    : entity(entity) {
    helper::AddComponent(entity, std::move(name));
    helper::AddComponent(entity, std::move(position));
}
void TransformNode::encode(YAML::Node& node) const {
    node["type"] = "TransformNode";
    node["id"] = GetEntity().GetId();
    auto name = helper::GetComponent<Name>(entity).get();
    node["name"] = name->name;
    node["position"] = *helper::GetComponent<Position>(entity).get();
}

    bool TransformNode::decode(const YAML::Node& node) {
    GetPosition()->decode(node["position"]);
    return true;
}

std::shared_ptr<Position> TransformNode::GetPosition() const {
    return voxie::helper::GetComponent<Position>(entity);
}

std::shared_ptr<Name> TransformNode::GetName() const {
    return voxie::helper::GetComponent<Name>(entity);
}

const Entity &TransformNode::GetEntity() const {
    return entity;
}

}
