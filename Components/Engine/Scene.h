//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include <iostream>
#include <list>
#include <EntityComponentSystem.h>
#include <unordered_map>

namespace voxie {
    class Skybox;
    class Node;
    class NodeWrapper;
    class LightSource;
    class Shader;

    class Scene {
    public:
        Scene(const std::string &folder);
        ~Scene();
        void SetFilename(const std::string &name);
        void Save() const;
        void SaveAs(const std::string &name);
        void Load(const std::string &name);
        void ClearScene();
        Node * AddEntity(std::shared_ptr<NodeWrapper>, Node * parent = nullptr);
        Node * AddEntity(Handle, Node * parent = nullptr);
        void DisableEntity(const Handle&);
        void EnableEntity(const Handle&);

        template <typename T>
        void AddNode(std::shared_ptr<T> nodeWrapper, Node * parent) {
            auto rootNode = parent ? parent : GetRoot();
            auto node = std::make_unique<Node>(std::dynamic_pointer_cast<NodeWrapper>(std::move(nodeWrapper)), rootNode);
            AddNodeImplementation(std::move(node), rootNode);
        }
        using SceneEntities = std::list<Handle>;
        void RemoveEntity(Handle);
        SceneEntities GetEntities() const;

        using SceneNodes = std::list<std::shared_ptr<NodeWrapper>>;
        SceneNodes GetNodesForRendering() const;
        std::shared_ptr<NodeWrapper> FindNode(const Handle &);

        const std::string &GetSceneName() const;
        Node * GetRoot() const;
        Skybox* GetSkybox() const;

        void UpdateLights() const;

    private:
        void AddNodeImplementation(std::unique_ptr<Node>&& node, Node* rootNode);

        bool IsAffectedByLight(std::shared_ptr<NodeWrapper>) const;
        void UpdateLightSources(std::shared_ptr<Shader>, const std::vector<std::shared_ptr<LightSource>>&) const;

        std::unique_ptr<Node> root;
        std::string folder;
        std::string sceneName;
        std::unique_ptr<Skybox> skybox;
        std::unordered_map<Handle, Node*> nodes;
    };

}// namespace voxie
