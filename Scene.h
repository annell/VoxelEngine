//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include <EntityComponentSystem.h>
#include <list>

namespace voxie {
    class SceneNode {
    public:
        SceneNode(Entity node)
        : node(node) {
        }

        const Entity& GetNode() const {
            return node;
        }

        void AddChild(std::unique_ptr<SceneNode>&& child) {
            children.push_back(std::move(child));
        }

        void RemoveChild(const Entity& childEntity) {
            auto entityPredicate = [&] (std::unique_ptr<SceneNode>& child) {
                return child->GetNode() == childEntity;
            };
            auto new_end = std::remove_if(children.begin(), children.end(), entityPredicate);
            children.erase(new_end, children.end());
        }

        void TransferChild(Entity childEntity, SceneNode& target) {
            /*
            if (auto childNode = RemoveChild(childEntity)) {
                target.AddChild(std::move(childNode));
            }
            */
        }

        std::list<Entity> GetChildEntities() const {
            std::list<Entity> entities = { GetNode() };
            for (auto& child : children) {
                entities.merge(child->GetChildEntities());
            }
            return entities;
        }
    private:
        Entity node;
        std::list<std::unique_ptr<SceneNode>> children;
    };

    class Scene {
    public:
        Scene(const std::string &folder);
        void SetFilename(const std::string &name);
        void Save() const;
        void SaveAs(const std::string &name);
        void Load(const std::string &name);
        using SceneEntities = std::list<Entity>;
        void ClearScene();
        void AddEntity(Entity);
        void RemoveEntity(Entity);
        SceneEntities GetEntities() const;

        const std::string &GetSceneName() const;

    private:
        std::unique_ptr<SceneNode> root;
        std::string folder;
        std::string sceneName;
    };

}// namespace voxie
