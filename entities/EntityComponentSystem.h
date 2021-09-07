//
// Created by Stefan Annell on 2021-05-12.
//

#pragma once
#include <algorithm>
#include <map>
#include <string>

namespace voxie {

    class Entity {
    public:
        using Identity = int;
        Entity(Identity);

        static Entity MakeEntity();
        const Identity &GetId() const;

        bool operator<(const Entity &rhs) const;

        bool operator==(const Entity &rhs) const;

    private:
        auto as_tie() const;

        Entity::Identity id;
    };

    class EntityComponentSystem {
    public:
        template<typename T>
        std::shared_ptr<T> GetComponent(const Entity &handle);

        template<typename T>
        void AddComponent(const Entity &handle, std::shared_ptr<T> component);

        template<typename T>
        void RemoveComponent(const Entity &handle);

    private:
    };

    template<typename T>
    std::map<Entity, std::shared_ptr<T>> &GetComponents() {
        static std::map<Entity, std::shared_ptr<T>> entities;
        return entities;
    }

    template<typename T>
    std::shared_ptr<T> EntityComponentSystem::GetComponent(const Entity &handle) {
        auto &components = GetComponents<T>();
        auto it = components.find(handle);
        if (it != components.end()) {
            return it->second;
        }
        return nullptr;
    }

    template<typename T>
    void EntityComponentSystem::AddComponent(const Entity &handle, std::shared_ptr<T> component) {
        GetComponents<T>()[handle] = component;
    }

    template<typename T>
    void EntityComponentSystem::RemoveComponent(const Entity &handle) {
        auto components = GetComponents<T>();
        auto it = components.find(handle);
        if (it != components.end()) {
            components.erase(it);
        }
    }

}// namespace voxie
