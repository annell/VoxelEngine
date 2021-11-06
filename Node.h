//
// Created by Stefan Annell on 2021-10-16.
//

#pragma once
#include "BaseComponents.h"
#include "Core.h"
#include <EntityComponentSystem.h>

namespace voxie {

struct NodeWrapper {
    virtual ~NodeWrapper() {}
    virtual const Entity& GetEntity() const = 0;
    virtual std::shared_ptr<Position> GetPosition() const {
        return std::shared_ptr<Position>();
    }
};

class Node {
public:
    Node(Entity node, Node * parent);
    Node(std::shared_ptr<NodeWrapper>, Node * parent);
    ~Node();

    void encode(YAML::Node &) const;
    bool decode(const YAML::Node &);

    const Entity& GetNode() const;
    std::shared_ptr<NodeWrapper> GetNodePtr() const;
    void AddChild(std::unique_ptr<Node>&& child);
    std::unique_ptr<Node> RemoveChild(const Entity& childEntity);
    void MoveTo(Node * target);
    Node* Find(const Entity& entity);
    std::shared_ptr<NodeWrapper> FindNode(const Entity& entity);

    const std::list<std::unique_ptr<Node>>& GetChildNodes() const;
    size_t GetNumChildren() const;
    std::list<Entity> GetChildEntities() const;
    Node * GetParent() const;
    glm::vec3 GetRelativePosition();

private:
    void MoveChild(Node * child, Node * target);

    Node * parent = nullptr;
    Entity node;
    std::shared_ptr<NodeWrapper> nodePtr;
    std::list<std::unique_ptr<Node>> children;
};

}
