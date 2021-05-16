//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include <vector>
#include <EntityComponentSystem.h>

namespace engine {


class Scene {
public:
    using SceneEntities = std::vector<std::shared_ptr<entities::Entity>>;
    void AddEntity(std::shared_ptr<entities::Entity>);
    const SceneEntities& GetEntities();
private:
    SceneEntities entities;
};

}
