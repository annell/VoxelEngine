//
// Created by Stefan Annell on 2021-05-16.
//

#include "Scene.h"

void engine::Scene::AddEntity(std::shared_ptr<entities::Entity> entity) {
    entities.push_back(entity);
}

const engine::Scene::SceneEntities &engine::Scene::GetEntities() {
    return entities;
}
