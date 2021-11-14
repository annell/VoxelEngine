//
// Created by Stefan Annell on 2021-10-16.
//

#include "Node.h"
#include "Engine.h"
#include "Factory.h"
#include <Sprite.h>
namespace voxie {

Node::Node(Handle node, Node * parent)
    : handle(std::move(node))
    , nodePtr({})
    , parent(parent) {

}

Node::Node(std::shared_ptr<NodeWrapper> nodeWrapper, Node * parent)
    : handle(nodeWrapper->GetEntity())
    , nodePtr(nodeWrapper)
    , parent(parent) {

}

Node::~Node() {

}

void Node::encode(YAML::Node &root) const {
    YAML::Node node;
    if (parent) {
        node["parent"] = parent->GetHandle().GetId();
    }
    auto handle = GetHandle();
    if (auto camera = helper::GetSceneNode<Camera>(handle)) {
        node["node"] = *camera.get();
    } else if (auto chunk = helper::GetSceneNode<Chunk>(handle)) {
        node["node"] = *chunk.get();
    } else if (auto light = helper::GetSceneNode<LightSource>(handle)) {
        node["node"] = *light.get();
    } else if (auto sprite = helper::GetSceneNode<Sprite>(handle)) {
        node["node"] = *sprite.get();
    } else if (auto sprite = helper::GetSceneNode<TransformNode>(handle)) {
        node["node"] = *sprite.get();
    }
    root.push_back(node);

    for (auto & child : children) {
        child->encode(root);
    }
}

bool Node::decode(const YAML::Node &n) {
    auto nodeEntity = Handle(n["id"].as<int>());
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

const Handle &Node::GetHandle() const {
    return handle;
}

void Node::AddChild(std::unique_ptr<Node>&& child) {
    children.push_back(std::move(child));
}

std::unique_ptr<Node> Node::RemoveChild(const Handle & childEntity) {
    for (auto it = children.begin(); it != children.end(); it++) {
        if (it->get()->GetHandle() == childEntity) {
            auto out = std::move(*it);
            children.erase(it);
            return std::move(out);
        }
    }
    return {};
}

void Node::MoveTo(Node * target) {
    if (!Find(target->GetHandle())) {
        parent->MoveChild(this, target);
        parent = target;
    }
}

void Node::MoveChild(Node * child, Node * target) {
    auto childEntity = child->GetHandle();
    for (auto it = children.begin(); it != children.end(); it++) {
        if (it->get()->GetHandle() == childEntity) {
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

std::list<Handle> Node::GetChildEntities() const {
    std::list<Handle> entities = {GetHandle() };
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

std::shared_ptr<NodeWrapper> Node::FindNode(const Handle &handle) {
    if (auto node = Find(handle)) {
        return node->GetNodePtr();
    }
    return nullptr;
}

Node* Node::Find(const Handle &handle) {
    if (this->handle == handle) {
        return this;
    }
    for (const auto& child : children) {
        if (auto childNode = child->Find(handle)) {
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
