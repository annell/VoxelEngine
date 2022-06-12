//
// Created by Stefan Annell on 2021-10-16.
//

#pragma once

#include "Handle.h"
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

#include "Engine.h"

#define COMPONENT_ADD_FUNCTION(TYPE)                           \
    std::shared_ptr<TYPE> Get##TYPE() const {                  \
        return voxie::helper::GetComponent<TYPE>(GetHandle()); \
    }

#define COMPONENT_REGISTER(TYPE, COMPONENT)                  \
    helper::AddComponent(GetHandle(), std::move(COMPONENT)); \
    componentsToRemove.push_back([&]() { helper::RemoveComponent<TYPE>(GetHandle()); });

namespace voxie {
    class Position;

    struct NodeWrapper {
        NodeWrapper(Handle handle) : handle(handle) {}
        virtual ~NodeWrapper() {
            for (const auto &component : componentsToRemove) {
                component();
            }
        }
        const Handle &GetHandle() const { return handle; }
        virtual RenderingConfig GetRenderingConfig() const { return {}; }

        bool IsEnabled() const { return enabled; }
        void Enable() { enabled = true; }
        void Disable() { enabled = false; }

    protected:
        bool enabled = true;
        Handle handle;
        std::vector<std::function<void()>> componentsToRemove;
    };

    class Node {
    public:
        Node(Handle, Node *parent);
        Node(std::shared_ptr<NodeWrapper>, Node *parent);
        ~Node();

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        const Handle &GetHandle() const;
        std::shared_ptr<NodeWrapper> GetNodePtr() const;
        void AddChild(std::unique_ptr<Node> &&child);
        std::unique_ptr<Node> RemoveChild(const Handle &childEntity);
        void MoveTo(Node *target);
        Node *Find(const Handle &handle);
        std::shared_ptr<NodeWrapper> FindNode(const Handle &handle);

        const std::list<std::unique_ptr<Node>> &GetChildNodes() const;
        size_t GetNumChildren() const;
        std::list<Handle> GetChildEntities() const;
        Node *GetParent() const;

    private:
        void MoveChild(Node *child, Node *target);

        Node *parent = nullptr;
        Handle handle;
        std::shared_ptr<NodeWrapper> nodePtr;
        std::list<std::unique_ptr<Node>> children;
    };

}// namespace voxie
