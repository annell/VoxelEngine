//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include "Node.h"
#include <BaseComponents.h>
#include <EntityComponentSystem.h>
#include <iostream>
#include <list>

namespace voxie {

    class Scene {
    public:
        Scene(const std::string &folder);
        void SetFilename(const std::string &name);
        void Save() const;
        void SaveAs(const std::string &name);
        void Load(const std::string &name);
        using SceneEntities = std::list<Entity>;
        void ClearScene();
        Node * AddEntity(Entity, Node * parent = nullptr);

        template <typename T>
        void AddNode(std::shared_ptr<T> nodeWrapper, Node * parent) {
            auto rootNode = parent ? parent : GetRoot();
            auto node = std::make_unique<Node>(std::dynamic_pointer_cast<NodeWrapper>(std::move(nodeWrapper)), rootNode);
            if (rootNode) {
                rootNode->AddChild(std::move(node));
            } else {
                root = std::move(node);
            }
        }
        void RemoveEntity(Entity);
        SceneEntities GetEntities() const;
        std::shared_ptr<NodeWrapper> FindNode(const Entity&);

        const std::string &GetSceneName() const;
        Node * GetRoot() const;

    private:
        std::unique_ptr<Node> root;
        std::string folder;
        std::string sceneName;
    };

}// namespace voxie
