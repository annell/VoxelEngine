//
// Created by Stefan Annell on 2021-10-16.
//

#include "Node.h"
#include "Engine.h"
#include "Factory.h"
#include <Sprite.h>
namespace voxie {

Node::Node(Entity node, Node * parent)
    : node(std::move(node))
    , nodePtr({})
    , parent(parent) {

}

Node::Node(std::shared_ptr<NodeWrapper> nodeWrapper, Node * parent)
    : node(nodeWrapper->GetEntity())
    , nodePtr(nodeWrapper)
    , parent(parent) {

}

Node::~Node() {

}

void Node::encode(YAML::Node &root) const {
    YAML::Node node;
    if (parent) {
        node["parent"] = parent->GetNode().GetId();
    }
    auto entity = GetNode();
    if (auto camera = helper::GetSceneNode<Camera>(entity)) {
        node["node"] = *camera.get();
    } else if (auto chunk = helper::GetSceneNode<Chunk>(entity)) {
        node["node"] = *chunk.get();
    } else if (auto light = helper::GetSceneNode<LightSource>(entity)) {
        node["node"] = *light.get();
    } else if (auto sprite = helper::GetSceneNode<Sprite>(entity)) {
        node["node"] = *sprite.get();
    } else if (auto sprite = helper::GetSceneNode<TransformNode>(entity)) {
        node["node"] = *sprite.get();
    }
    root.push_back(node);

    for (auto & child : children) {
        child->encode(root);
    }
}

bool Node::decode(const YAML::Node &n) {
    auto nodeEntity = Entity(n["id"].as<int>());
    if (n["type"].as<std::string>() == "Chunk") {
        auto obj = MakeModel({n["name"].as<std::string>(),
                              n["path"].as<std::string>(),
                              nodeEntity});
        obj->decode(n);
        nodePtr = std::move(obj);
    } else if (n["type"].as<std::string>() == "Camera") {
        auto obj = MakeCamera({n["name"].as<std::string>(),
                               nodeEntity});
        obj->decode(n);
        auto entity = obj->GetEntity();
        nodePtr = std::move(obj);
        if (n["activeCamera"].as<bool>()) {
            Engine::GetEngine().SetCamera(entity);
        }
    } else if (n["type"].as<std::string>() == "LightSource") {
        auto obj = MakeLight({n["name"].as<std::string>(),
                              (LightType) n["lightType"].as<int>(),
                              nodeEntity});
        obj->decode(n);
        nodePtr = std::move(obj);
    } else if (n["type"].as<std::string>() == "Sprite") {
        auto obj = MakeSprite({n["name"].as<std::string>(),
                               n["path"].as<std::string>(),
                               nodeEntity});
        obj->decode(n);
        nodePtr = std::move(obj);
    } else if (n["type"].as<std::string>() == "TransformNode") {
        auto obj = MakeTransformNode({n["name"].as<std::string>(),
                                       nodeEntity});
        obj->decode(n);
        nodePtr = std::move(obj);
    }
    return true;
}

const Entity&Node::GetNode() const {
    return node;
}

void Node::AddChild(std::unique_ptr<Node>&& child) {
    children.push_back(std::move(child));
}

std::unique_ptr<Node> Node::RemoveChild(const Entity& childEntity) {
    for (auto it = children.begin(); it != children.end(); it++) {
        if (it->get()->GetNode() == childEntity) {
            auto out = std::move(*it);
            children.erase(it);
            return std::move(out);
        }
    }
    return {};
}

void Node::MoveTo(Node * target) {
    if (!Find(target->GetNode())) {
        parent->MoveChild(this, target);
        parent = target;
    }
}

void Node::MoveChild(Node * child, Node * target) {
    auto childEntity = child->GetNode();
    for (auto it = children.begin(); it != children.end(); it++) {
        if (it->get()->GetNode() == childEntity) {
            auto childPtr = std::move(*it);
            children.erase(it);
            target->AddChild(std::move(childPtr));
            break;
        }
    }
}

const std::list<std::unique_ptr<Node>>&Node::GetChildNodes() const {
    return children;
}

size_t Node::GetNumChildren() const {
    return children.size();
}

std::list<Entity> Node::GetChildEntities() const {
    std::list<Entity> entities = { GetNode() };
    for (auto& child : GetChildNodes()) {
        if (child) {
            entities.merge(child->GetChildEntities());
        }
    }
    return entities;
}

Node *Node::GetParent() const {
    return parent;
}

std::shared_ptr<NodeWrapper> Node::FindNode(const Entity& entity) {
    if (auto node = Find(entity)) {
        return node->GetNodePtr();
    }
    return nullptr;
}

Node* Node::Find(const Entity& entity) {
    if (entity == node) {
        return this;
    }
    for (const auto& child : children) {
        if (auto childNode = child->Find(entity)) {
            return childNode;
        }
    }
    return nullptr;
}
std::shared_ptr<NodeWrapper> Node::GetNodePtr() const {
    return nodePtr;
}

glm::vec3 Node::GetRelativePosition() {
    if (!parent) {
        return GetNodePtr()->GetPosition()->pos;
    }
    return parent->GetRelativePosition() + GetNodePtr()->GetPosition()->pos;
}

glm::vec3 Node::GetRelativeRotation() {
    if (!parent) {
        return GetNodePtr()->GetPosition()->rotation;
    }
    return parent->GetRelativeRotation() + GetNodePtr()->GetPosition()->rotation;
}

}
