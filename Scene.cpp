//
// Created by Stefan Annell on 2021-05-16.
//

#include "Scene.h"
#include "Engine.h"

namespace voxie {
    void Scene::Save() const {
        YAML::Node node;
        for (const auto& entity : entities) {
            if (auto camera = helper::GetComponent<Camera>(entity)) {
                node[entity.GetId()].push_back(*camera.get());
            } else if (auto chunk = helper::GetComponent<Chunk>(entity)) {
                node[entity.GetId()].push_back(*chunk.get());
            }
        }
        std::ofstream fout("config.yaml");
        fout << node;
    }

    void Scene::AddEntity(Entity entity) {
        entities.push_back(entity);
    }

    const Scene::SceneEntities &Scene::GetEntities() {
        return entities;
    }

    void Scene::RemoveEntity(Entity entity) {
        for (auto it = entities.begin(); it != entities.end(); it++) {
            if (*it == entity) {
                entities.erase(it);
                break;
            }
        }
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    }

}// namespace voxie
