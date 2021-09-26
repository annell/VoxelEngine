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
        while (stream.read(& buf[0], read_size)) {
            out.append(buf, 0, stream.gcount());
        }
        out.append(buf, 0, stream.gcount());
        return out;
    };

}

namespace voxie {
    Scene::Scene(const std::string& folder)
    : folder(folder) {
    }

    void Scene::SetFilename(const std::string &name) {
        sceneName = name;
    }

    void Scene::Save() const {
        YAML::Node node;
        for (const auto& entity : entities) {
            if (auto camera = helper::GetComponent<Camera>(entity)) {
                node.push_back(*camera.get());
            } else if (auto chunk = helper::GetComponent<Chunk>(entity)) {
                node.push_back(*chunk.get());
            } else if (auto light = helper::GetComponent<LightSource>(entity)) {
                node.push_back(*light.get());
            } else if (auto sprite = helper::GetComponent<Sprite>(entity)) {
                node.push_back(*sprite.get());
            }
        }
        std::ofstream fout(folder + sceneName);
        fout << node;
    }
    void Scene::SaveAs(const std::string& name) {
        SetFilename(name);
        Save();
    }

    void Scene::Load(const std::string& name) {
        SetFilename(name);
        ClearScene();
        auto node = YAML::Load(internal::read_file(folder + sceneName));
        for (const auto& n : node) {
            if (n["type"].as<std::string>() == "Chunk") {
                auto obj = MakeModel({
                    n["name"].as<std::string>(),
                    n["path"].as<std::string>()
                });
                obj->decode(n);
                auto entity = obj->GetEntity();
                voxie::helper::AddComponent(entity, std::move(obj));
            } else if (n["type"].as<std::string>() == "Camera") {
                auto obj = MakeCamera({
                    n["name"].as<std::string>()
                });
                obj->decode(n);
                auto entity = obj->GetEntity();
                voxie::helper::AddComponent(entity, std::move(obj));
                if (n["activeCamera"].as<bool>()) {
                    Engine::GetEngine().SetCamera(entity);
                }
            } else if (n["type"].as<std::string>() == "LightSource") {
                auto obj = MakeLight({
                  n["name"].as<std::string>(),
                  (LightType)n["lightType"].as<int>()
                });
                obj->decode(n);
                auto entity = obj->GetEntity();
                voxie::helper::AddComponent(entity, std::move(obj));
            } else if (n["type"].as<std::string>() == "Sprite") {
                auto obj = MakeSprite({
                  n["name"].as<std::string>(),
                  n["path"].as<std::string>()
                });
                obj->decode(n);
                auto entity = obj->GetEntity();
                voxie::helper::AddComponent(entity, std::move(obj));
            }
        }
    }

    void Scene::AddEntity(Entity entity) {
        entities.push_back(entity);
    }

    const Scene::SceneEntities &Scene::GetEntities() {
        return entities;
    }

    void Scene::RemoveEntity(Entity entity) {
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
        if(helper::HasComponent<Chunk>(entity)) {
            helper::RemoveComponent<Chunk>(entity);
        } else if (helper::HasComponent<Sprite>(entity)) {
            helper::RemoveComponent<Sprite>(entity);
        } else if (helper::HasComponent<LightSource>(entity)) {
            helper::RemoveComponent<LightSource>(entity);
        }
    }
    void Scene::ClearScene() {
        auto entities = GetEntities();
        for (const auto& entity : entities) {
            RemoveEntity(entity);
        }
    }
    const std::string& Scene::GetSceneName() const {
        return sceneName;
    }

}// namespace voxie
