//
// Created by Stefan Annell on 2021-05-12.
//

#pragma once
#include "BaseComponents.h"
#include <algorithm>
#include <vector>
#include <string>

namespace voxie {

    class Entity {
    public:
        using Identity = int;
        explicit Entity(Identity);

        static Entity MakeEntity();

        Entity::Identity GetId() const;

        bool operator<(const Entity &rhs) const;

        bool operator==(const Entity &rhs) const;

    private:
        [[nodiscard]] auto as_tie() const;

        Entity::Identity id;
    };
    static const Entity NullEntity(0);

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
    struct Component {
        Entity entity;
        std::shared_ptr<T> component;

        bool operator==(const Component<T>& rhs) {
            return this->entity == rhs.entity;
        }
    };

    template<typename T>
    std::vector<Component<T>> &GetComponents() {
        static std::vector<Component<T>> components;
        return components;
    }

    template<typename T>
    std::shared_ptr<T> EntityComponentSystem::GetComponent(const Entity &handle) {
        auto &components = GetComponents<T>();
        for (auto it = components.begin(); it != components.end(); it++) {
            if (it->entity == handle) {
                return it->component;
            }
        }
        return nullptr;
    }

    template<typename T>
    void EntityComponentSystem::AddComponent(const Entity &handle, std::shared_ptr<T> component) {
        auto &components = GetComponents<T>();
        components.push_back({handle, std::move(component)});
    }

    template<typename T>
    void EntityComponentSystem::RemoveComponent(const Entity &handle) {
        auto &components = GetComponents<T>();
        for (auto it = components.begin(); it != components.end(); it++) {
            if (it->entity == handle) {
                components.erase(it);
                return;
            }
        }
    }

}// namespace voxie
