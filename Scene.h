//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include <EntityComponentSystem.h>
#include <vector>

namespace voxie {

    class Scene {
    public:
        Scene(std::string folder);
        void Save() const;
        void SaveAs(std::string name);
        void Load(std::string name);
        using SceneEntities = std::vector<Entity>;
        void ClearScene();
        void AddEntity(Entity);
        void RemoveEntity(Entity);
        const SceneEntities &GetEntities();

    private:
        SceneEntities entities;
        std::string folder;
        std::string sceneName;
    };

}// namespace voxie
