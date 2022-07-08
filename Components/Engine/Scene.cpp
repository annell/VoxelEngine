//
// Created by Stefan Annell on 2021-05-16.
//

#include "Scene.h"
#include "Factory.h"
#include "Node.h"
#include "Shader.h"
#include <Skybox.h>
#include <fstream>

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
        ClearScene();
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
        auto saveFile = YAML::Load(internal::read_file(folder + sceneName));
        for (const auto &node : saveFile) {
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
        UpdateLights();
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

    void Scene::AddNodeImplementation(std::unique_ptr<Node> &&node, Node *rootNode) {
        nodes[node->GetHandle()] = node.get();
        if (rootNode) {
            rootNode->AddChild(std::move(node));
        } else {
            root = std::move(node);
        }
    }

    Scene::SceneEntities Scene::GetEntities() const {
        SceneEntities keys;
        for (const auto &node : nodes) {
            keys.push_back(node.first);
        }
        return keys;
    }

    Scene::SceneNodes Scene::GetNodesForRendering() const {
        auto lightSources = helper::GetSceneNodes<LightSource>(GetEntities());
        auto camera = Engine::GetEngine().GetCamera();

        Scene::SceneNodes nodesForRendering;
        for (const auto &node : nodes) {
            auto nodePtr = node.second->GetNodePtr();
            if (nodePtr->IsEnabled()) {
                if (std::dynamic_pointer_cast<Chunk>(nodePtr) ||
                    std::dynamic_pointer_cast<voxie::Sprite>(nodePtr) ||
                    std::dynamic_pointer_cast<voxie::CubeEntity>(nodePtr) ||
                    std::dynamic_pointer_cast<voxie::Text>(nodePtr)) {
                    if (IsAffectedByLight(nodePtr)) {
                        auto shader = helper::GetComponent<Shader>(nodePtr->GetHandle());
                        camera->SetShaderParameters(*shader);
                        shader->setBool("selected", nodePtr->GetHandle() == camera->GetSelection());
                        if (auto invertedHull = helper::GetComponent<Outline>(nodePtr->GetHandle())) {
                            camera->SetShaderParameters(*invertedHull->shader);
                        }
                    }
                    nodesForRendering.push_back(nodePtr);
                }
            }
        }
        return nodesForRendering;
    }

    Scene::SceneNodes Scene::GetNodesPtrs() const {
        Scene::SceneNodes nodePtrs;
        for (const auto &node : nodes) {
            auto nodePtr = node.second->GetNodePtr();
            nodePtrs.push_back(nodePtr);
        }
        return nodePtrs;
    }

    void Scene::RemoveEntity(Handle handle) {
        nodes.erase(handle);
        if (root) {
            root->RemoveChild(handle);
        }
    }
    void Scene::ClearScene() {
        root.reset();
        nodes.clear();
    }
    const std::string &Scene::GetSceneName() const {
        return sceneName;
    }

    Node *Scene::GetRoot() const {
        return root.get();
    }

    std::shared_ptr<NodeWrapper> Scene::FindNode(const Handle &entity) {
        auto it = nodes.find(entity);
        if (it != nodes.end()) {
            return it->second->GetNodePtr();
        }
        return nullptr;
    }

    Skybox *Scene::GetSkybox() const {
        return skybox.get();
    }

    void Scene::DisableEntity(const Handle &handle) {
        if (auto node = FindNode(handle)) {
            node->Disable();
        }
    }

    void Scene::EnableEntity(const Handle &handle) {
        if (auto node = FindNode(handle)) {
            node->Enable();
        }
    }

    bool Scene::IsAffectedByLight(std::shared_ptr<NodeWrapper> node) const {
        return std::dynamic_pointer_cast<Chunk>(node) ||
               std::dynamic_pointer_cast<CubeEntity>(node);
    }

    void Scene::UpdateLightSources(std::shared_ptr<Shader> shader, const std::vector<std::shared_ptr<LightSource>> &lightSources) const {
        shader->use();
        shader->setInt("nrLights", lightSources.size());
        int n = 0;

        for (const auto &light : lightSources) {
            std::string index = std::to_string(n);
            shader->setVec3("lights[" + index + "].lightColor", light->GetColor()->color);
            shader->setVec3("lights[" + index + "].lightPos", light->GetPosition()->pos);
            shader->setInt("lights[" + index + "].type", static_cast<int>(light->GetType()));
            if (light->GetAttenuation()) {
                shader->setFloat("lights[" + index + "].constant", light->GetAttenuation()->constant);
                shader->setFloat("lights[" + index + "].linear", light->GetAttenuation()->linear);
                shader->setFloat("lights[" + index + "].quadratic", light->GetAttenuation()->quadratic);
            }
            n++;
        }
    }

    void Scene::UpdateLight(std::shared_ptr<NodeWrapper> node) const {
        if (IsAffectedByLight(node)) {
            UpdateLightSources(helper::GetComponent<Shader>(node->GetHandle()), helper::GetSceneNodes<LightSource>(GetEntities()));
        }
    }

    void Scene::UpdateLights() const {
        for (const auto &node : GetNodesForRendering()) {
            UpdateLight(node);
        }
    }

}// namespace voxie
