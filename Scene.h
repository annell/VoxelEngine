//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include <EntityComponentSystem.h>
#include <list>
#include <iostream>

namespace voxie {
    class SceneNode {
    public:
        SceneNode(Entity node, SceneNode* parent)
        : node(std::move(node))
        , parent(parent) {

        }


        ~SceneNode() {

        }

        const Entity& GetNode() const {
            return node;
        }

        void AddChild(std::unique_ptr<SceneNode>&& child) {
            children.push_back(std::move(child));
        }

        std::unique_ptr<SceneNode>&& RemoveChild(const Entity& childEntity) {
            for (auto it = children.begin(); it != children.end(); it++) {
                if (it->get()->GetNode() == childEntity) {
                    auto out = std::move(*it);
                    children.erase(it);
                    return std::move(out);
                }
            }
            return {};
        }

        void Move(SceneNode* root) {
            parent->MoveChild(this, root);
            parent = root;
        }

        void MoveChild(SceneNode* child, SceneNode* target) {
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

        const std::list<std::unique_ptr<SceneNode>>& GetChildNodes() const {
            return children;
        }

        size_t GetNumChildren() const {
            return children.size();
        }

        std::list<Entity> GetChildEntities() const {
            std::list<Entity> entities = { GetNode() };
            for (auto& child : GetChildNodes()) {
                if (child) {
                    entities.merge(child->GetChildEntities());
                }
            }
            return entities;
        }

        SceneNode* Find(const Entity& entity) {
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

    private:
        SceneNode* parent = nullptr;
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
        SceneNode* FindNode(const Entity&);

        const std::string &GetSceneName() const;
        SceneNode* GetRoot() const;

    private:
        std::unique_ptr<SceneNode> root;
        std::string folder;
        std::string sceneName;
    };

}// namespace voxie
