//
// Created by Stefan Annell on 2021-05-16.
//

#pragma once
#include <EntityComponentSystem.h>
#include <vector>

namespace voxie {

    class Scene {
    public:
        Scene(const std::string &folder);
        void SetFilename(const std::string &name);
        void Save() const;
        void SaveAs(const std::string &name);
        void Load(const std::string &name);
        using SceneEntities = std::vector<Entity>;
        void ClearScene();
        void AddEntity(Entity);
        void RemoveEntity(Entity);
        const SceneEntities &GetEntities();

        const std::string &GetSceneName() const;

    private:
        SceneEntities entities;
        std::string folder;
        std::string sceneName;
    };

}// namespace voxie
