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

    SceneNode::SceneNode(Entity node, SceneNode* parent)
    : node(std::move(node))
    , parent(parent) {

    }

    void SceneNode::encode(YAML::Node &root) const {
        YAML::Node node;
        if (parent) {
            node["parent"] = parent->GetNode().GetId();
        }
        auto entity = GetNode();
        if (auto camera = helper::GetComponent<Camera>(entity)) {
            node["node"] = *camera.get();
        } else if (auto chunk = helper::GetComponent<Chunk>(entity)) {
            node["node"] = *chunk.get();
        } else if (auto light = helper::GetComponent<LightSource>(entity)) {
            node["node"] = *light.get();
        } else if (auto sprite = helper::GetComponent<Sprite>(entity)) {
            node["node"] = *sprite.get();
        }
        root.push_back(node);

        for (auto & child : children) {
            child->encode(root);
        }
    }

    bool SceneNode::decode(const YAML::Node &n) {
        auto nodeEntity = Entity(n["id"].as<int>());
        if (n["type"].as<std::string>() == "Chunk") {
            auto obj = MakeModel({n["name"].as<std::string>(),
                                  n["path"].as<std::string>(),
                                          nodeEntity});
            obj->decode(n);
            auto entity = obj->GetEntity();
            voxie::helper::AddComponent(entity, std::move(obj));
        } else if (n["type"].as<std::string>() == "Camera") {
            auto obj = MakeCamera({n["name"].as<std::string>(),
                                    nodeEntity});
            obj->decode(n);
            auto entity = obj->GetEntity();
            voxie::helper::AddComponent(entity, std::move(obj));
            if (n["activeCamera"].as<bool>()) {
                Engine::GetEngine().SetCamera(entity);
            }
        } else if (n["type"].as<std::string>() == "LightSource") {
            auto obj = MakeLight({n["name"].as<std::string>(),
                                  (LightType) n["lightType"].as<int>(),
                                 nodeEntity});
            obj->decode(n);
            auto entity = obj->GetEntity();
            voxie::helper::AddComponent(entity, std::move(obj));
        } else if (n["type"].as<std::string>() == "Sprite") {
            auto obj = MakeSprite({n["name"].as<std::string>(),
                                   n["path"].as<std::string>(),
                                   nodeEntity});
            obj->decode(n);
            auto entity = obj->GetEntity();
            voxie::helper::AddComponent(entity, std::move(obj));
        }
        return true;
    }

    const Entity& SceneNode::GetNode() const {
        return node;
    }

    void SceneNode::AddChild(std::unique_ptr<SceneNode>&& child) {
        children.push_back(std::move(child));
    }

    std::unique_ptr<SceneNode>&& SceneNode::RemoveChild(const Entity& childEntity) {
        for (auto it = children.begin(); it != children.end(); it++) {
            if (it->get()->GetNode() == childEntity) {
                auto out = std::move(*it);
                children.erase(it);
                return std::move(out);
            }
        }
        return {};
    }

    void SceneNode::MoveTo(SceneNode* target) {
        parent->MoveChild(this, target);
        parent = target;
    }

    void SceneNode::MoveChild(SceneNode* child, SceneNode* target) {
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

    const std::list<std::unique_ptr<SceneNode>>& SceneNode::GetChildNodes() const {
        return children;
    }

    size_t SceneNode::GetNumChildren() const {
        return children.size();
    }

    std::list<Entity> SceneNode::GetChildEntities() const {
        std::list<Entity> entities = { GetNode() };
        for (auto& child : GetChildNodes()) {
            if (child) {
                entities.merge(child->GetChildEntities());
            }
        }
        return entities;
    }

    SceneNode* SceneNode::GetParent() const {
        return parent;
    }

    SceneNode* SceneNode::Find(const Entity& entity) {
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
            SceneNode* parent = nullptr;
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

    SceneNode* Scene::AddEntity(Entity entity, SceneNode* parent) {
        auto rootNode = parent ? parent : GetRoot();
        auto node = std::make_unique<SceneNode>(entity, rootNode);
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
            if (helper::HasComponent<Chunk>(entity)) {
                helper::RemoveComponent<Chunk>(entity);
            } else if (helper::HasComponent<Sprite>(entity)) {
                helper::RemoveComponent<Sprite>(entity);
            } else if (helper::HasComponent<LightSource>(entity)) {
                helper::RemoveComponent<LightSource>(entity);
            }
        }
    }
    void Scene::ClearScene() {
        for (const auto &entity : GetEntities()) {
            RemoveEntity(entity);
        }
        if (root) {
            RemoveEntity(root->GetNode());
            root.reset();
        }
    }
    const std::string &Scene::GetSceneName() const {
        return sceneName;
    }

    SceneNode *Scene::GetRoot() const {
        return root.get();
    }
    SceneNode *Scene::FindNode(const Entity & entity) {
        if (root) {
            return root->Find(entity);
        }
        return nullptr;
    }

}// namespace voxie
