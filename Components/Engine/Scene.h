//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include "ChunkPos.h"
#include <EntityComponentSystem.h>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <unordered_map>

namespace voxie {
    class Skybox;
    class Node;
    class NodeWrapper;
    class LightSource;
    class Shader;
    class Position;

    class Scene {
    public:
        Scene(const std::string &folder);
        ~Scene();
        void SetFilename(const std::string &name);
        void Save() const;
        void SaveAs(const std::string &name);
        void Load(const std::string &name);
        void ClearScene();
        Node *AddEntity(std::shared_ptr<NodeWrapper>, Node *parent = nullptr);
        Node *AddEntity(Handle, Node *parent = nullptr);
        void DisableEntity(const Handle &);
        void EnableEntity(const Handle &);

        template<typename T>
        void AddNode(std::shared_ptr<T> nodeT, Node *parent) {
            auto nodeWrapper = std::dynamic_pointer_cast<NodeWrapper>(std::move(nodeT));
            auto node = AddEntity(nodeWrapper->GetHandle(), parent);
            node->nodePtr = nodeWrapper;
            UpdateLight(nodeWrapper);
        }
        using SceneEntities = std::list<Handle>;
        void RemoveEntity(Handle);
        SceneEntities GetEntities() const;

        using SceneNodes = std::list<std::shared_ptr<NodeWrapper>>;
        SceneNodes GetNodesForRendering() const;
        SceneNodes GetNodesPtrs() const;
        std::shared_ptr<NodeWrapper> FindNode(const Handle &);

        template<typename T>
        std::shared_ptr<T> FindEntity() {
            for (auto nodePtr : GetNodesPtrs()) {
                if (auto entity = std::dynamic_pointer_cast<T>(nodePtr)) {
                    return entity;
                }
            }
            return {};
        }

        const std::string &GetSceneName() const;
        Node *GetRoot() const;
        Skybox *GetSkybox() const;

        void UpdateLights() const;
        void RegisterPlayerController(const Handle &);

    private:
        void UpdateLight(std::shared_ptr<NodeWrapper>) const;
        void AddNodeImplementation(std::unique_ptr<Node> &&node, Node *rootNode);

        bool IsAffectedByLight(std::shared_ptr<NodeWrapper>) const;
        void UpdateLightSources(std::shared_ptr<Shader>, const std::vector<std::shared_ptr<LightSource>> &) const;

        void LoadWorldChunks(const Position &);

        std::unique_ptr<Node> root;
        std::string folder;
        std::string sceneName;
        std::unique_ptr<Skybox> skybox;
        std::unordered_map<Handle, Node *> nodes;
        std::vector<Handle> playerControllers;
        std::map<ChunkPos, Handle> loadedChunks;
    };

}// namespace voxie
