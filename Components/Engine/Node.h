//
// Created by Stefan Annell on 2021-10-16.
//

#pragma once

#include "Handle.h"
#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

namespace voxie {
class Position;

struct NodeWrapper {
    virtual ~NodeWrapper() {}
    virtual const Handle &GetHandle() const = 0;
    virtual std::shared_ptr<Position> GetPosition() const {
        return std::shared_ptr<Position>();
    }

    bool IsEnabled() const { return enabled; }
    void Enable() { enabled = true; }
    void Disable() { enabled = false; }
protected:
    bool enabled = true;
};

class Node {
public:
    Node(Handle, Node * parent);
    Node(std::shared_ptr<NodeWrapper>, Node * parent);
    ~Node();

    void encode(YAML::Node &) const;
    bool decode(const YAML::Node &);

    const Handle &GetHandle() const;
    std::shared_ptr<NodeWrapper> GetNodePtr() const;
    void AddChild(std::unique_ptr<Node>&& child);
    std::unique_ptr<Node> RemoveChild(const Handle & childEntity);
    void MoveTo(Node * target);
    Node* Find(const Handle &handle);
    std::shared_ptr<NodeWrapper> FindNode(const Handle &handle);

    const std::list<std::unique_ptr<Node>>& GetChildNodes() const;
    size_t GetNumChildren() const;
    std::list<Handle> GetChildEntities() const;
    Node * GetParent() const;
    glm::vec3 GetRelativePosition();
    glm::vec3 GetRelativeRotation();

private:
    void MoveChild(Node * child, Node * target);

    Node * parent = nullptr;
    Handle handle;
    std::shared_ptr<NodeWrapper> nodePtr;
    std::list<std::unique_ptr<Node>> children;
};

}
