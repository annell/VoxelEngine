//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include <vector>
#include <EntityComponentSystem.h>

namespace voxie {

class Scene {
public:
    using SceneEntities = std::vector<std::shared_ptr<Entity>>;
    void AddEntity(std::shared_ptr<Entity>);
    void RemoveEntity(std::shared_ptr<Entity>);
    const SceneEntities& GetEntities();
private:
    SceneEntities entities;
};

}
