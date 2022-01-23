//
// Created by Stefan Annell on 2021-05-16.
//

#include "Scene.h"
#include "Factory.h"
#include <fstream>
#include <Skybox.h>
#include "Node.h"
#include "Shader.h"

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
        : folder(folder), skybox(nullptr) {
    }

    Scene::~Scene() {
        root.reset();
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
            auto entity = Handle(node["node"]["id"].as<Handle::Identity>());
            Node *parent = nullptr;
            if (node["parent"].IsDefined()) {
                auto parentEntity = Handle(node["parent"].as<Handle::Identity>());
                if (root) {
                    parent = root->Find(parentEntity);
                }
            }
            auto newNode = AddEntity(entity, parent);
            newNode->decode(node["node"]);
        }

        skybox = std::make_unique<Skybox>(Handle::MakeEntity(), std::make_shared<voxie::Shader>(
        std::map<std::string, unsigned int>{
                std::make_pair(BASE_PATH + SHADERS + "/skybox.vs", GL_VERTEX_SHADER),
                std::make_pair(BASE_PATH + SHADERS + "/skybox.fs", GL_FRAGMENT_SHADER)}));
    }

    Node *Scene::AddEntity(std::shared_ptr<NodeWrapper> nodeWrapper, Node *parent) {
        auto rootNode = parent ? parent : GetRoot();
        auto node = std::make_unique<Node>(std::move(nodeWrapper), rootNode);
        auto nodePtr = node.get();
        AddNodeImplementation(std::move(node), rootNode);
        return nodePtr;
    }

    Node *Scene::AddEntity(Handle entity, Node *parent) {
        auto rootNode = parent ? parent : GetRoot();
        auto node = std::make_unique<Node>(entity, rootNode);
        auto nodePtr = node.get();
        AddNodeImplementation(std::move(node), rootNode);
        return nodePtr;
    }

    void Scene::AddNodeImplementation(std::unique_ptr<Node>&& node, Node* rootNode) {
        if (rootNode) {
            rootNode->AddChild(std::move(node));
        } else {
            root = std::move(node);
        }
    }

    Scene::SceneEntities Scene::GetEntities() const {
        if (root) {
            return root->GetChildEntities();
        }
        return {};
    }

    void Scene::RemoveEntity(Handle entity) {
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
    std::shared_ptr<NodeWrapper> Scene::FindNode(const Handle &entity) {
        if (root) {
            return root->FindNode(entity);
        }
        return nullptr;
    }
    Skybox* Scene::GetSkybox() const {
        return skybox.get();
    }

    void Scene::DisableEntity(const Handle& handle) {
        if (auto node = FindNode(handle)) {
            node->Disable();
        }
    }

    void Scene::EnableEntity(const Handle& handle) {
        if (auto node = FindNode(handle)) {
            node->Enable();
        }
    }

}// namespace voxie
