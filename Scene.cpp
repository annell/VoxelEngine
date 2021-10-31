//
// Created by Stefan Annell on 2021-05-16.
//

#include "Scene.h"
#include "Engine.h"
#include "Factory.h"

namespace internal {
    auto read_file(std::string_view path) -> std::string {
        constexpr auto read_size = std::size_t{4096};
        auto stream = std::ifstream{path.data()};
        stream.exceptions(std::ios_base::badbit);

        auto out = std::string{};
        auto buf = std::string(read_size, '\0');
        while (stream.read(&buf[0], read_size)) {
            out.append(buf, 0, stream.gcount());
        }
        out.append(buf, 0, stream.gcount());
        return out;
    };

}// namespace internal

namespace voxie {

    Scene::Scene(const std::string &folder)
            : folder(folder) {
    }

    void Scene::SetFilename(const std::string &name) {
        sceneName = name;
    }

    void Scene::Save() const {
        YAML::Node node;
        root->encode(node);
        std::ofstream fout(folder + sceneName);
        fout << node;
    }
    void Scene::SaveAs(const std::string &name) {
        SetFilename(name);
        Save();
    }

    void Scene::Load(const std::string &name) {
        SetFilename(name);
        ClearScene();
        auto nodes = YAML::Load(internal::read_file(folder + sceneName));
        for (const auto &node : nodes) {
            auto entity = Entity(node["node"]["id"].as<int>());
            Node * parent = nullptr;
            if (node["parent"].IsDefined()) {
                auto parentEntity = Entity(node["parent"].as<int>());
                if (root) {
                    parent = root->Find(parentEntity);
                }
            }
            auto newNode = AddEntity(entity, parent);
            newNode->decode(node["node"]);
        }
    }

    Node * Scene::AddEntity(Entity entity, Node * parent) {
        auto rootNode = parent ? parent : GetRoot();
        auto node = std::make_unique<Node>(entity, rootNode);
        auto nodePtr = node.get();
        if (rootNode) {
            rootNode->AddChild(std::move(node));
        } else {
            root = std::move(node);
        }
        return nodePtr;
    }

    Scene::SceneEntities Scene::GetEntities() const {
        if (root) {
            return root->GetChildEntities();
        }
        return {};
    }

    void Scene::RemoveEntity(Entity entity) {
        if (root) {
            root->RemoveChild(entity);
        }
    }
    void Scene::ClearScene() {
        root.reset(nullptr);
    }
    const std::string &Scene::GetSceneName() const {
        return sceneName;
    }

    Node *Scene::GetRoot() const {
        return root.get();
    }
    std::shared_ptr<NodeWrapper> Scene::FindNode(const Entity & entity) {
        if (root) {
            return root->FindNode(entity);
        }
        return nullptr;
    }

}// namespace voxie
