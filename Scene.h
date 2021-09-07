//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include <EntityComponentSystem.h>
#include <vector>

namespace voxie {

    class Scene {
    public:
        using SceneEntities = std::vector<Entity>;
        void AddEntity(Entity);
        void RemoveEntity(Entity);
        const SceneEntities &GetEntities();

    private:
        SceneEntities entities;
    };

}// namespace voxie
