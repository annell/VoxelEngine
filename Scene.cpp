//
// Created by Stefan Annell on 2021-05-16.
//

#include "Scene.h"

namespace voxie {

void Scene::AddEntity(std::shared_ptr<Entity> entity) {
    entities.push_back(entity);
}

const Scene::SceneEntities &Scene::GetEntities() {
    return entities;
}

}
