//
// Created by Stefan Annell on 2021-05-16.
//

#include "Scene.h"

namespace voxie {

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

}
