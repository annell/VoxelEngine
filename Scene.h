//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include <BaseComponents.h>
#include <EntityComponentSystem.h>
#include <iostream>
#include <list>

namespace voxie {
    class SceneNode {
    public:
        SceneNode(Entity node, SceneNode* parent);

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        const Entity& GetNode() const;
        void AddChild(std::unique_ptr<SceneNode>&& child);
        std::unique_ptr<SceneNode>&& RemoveChild(const Entity& childEntity);
        void MoveTo(SceneNode* target);
        SceneNode* Find(const Entity& entity);

        const std::list<std::unique_ptr<SceneNode>>& GetChildNodes() const;
        size_t GetNumChildren() const;
        std::list<Entity> GetChildEntities() const;
        SceneNode* GetParent() const;
    private:
        void MoveChild(SceneNode* child, SceneNode* target);

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
        SceneNode* AddEntity(Entity, SceneNode* parent = nullptr);
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
