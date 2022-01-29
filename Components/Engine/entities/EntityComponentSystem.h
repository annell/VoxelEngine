//
// Created by Stefan Annell on 2021-05-12.
//

#pragma once
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include "Handle.h"

namespace voxie {

    static const Handle NullEntity(0);

    class EntityComponentSystem {
    public:
        template<typename T>
        static std::shared_ptr<T> GetComponent(const Handle &handle);

        template<typename T>
        static void AddComponent(const Handle &handle, std::shared_ptr<T> component);

        template<typename T>
        static void RemoveComponent(Handle handle);

    private:
        EntityComponentSystem() {}
    };

    template<typename T>
    struct Component {
        Handle handle;
        std::shared_ptr<T> component;
    };

    template<typename T>
    std::unordered_map<Handle, Component<T>> &GetComponents() {
        static std::unordered_map<Handle, Component<T>> components;
        return components;
    }

    template<typename T>
    std::shared_ptr<T> EntityComponentSystem::GetComponent(const Handle &handle) {
        auto &components = GetComponents<T>();
        auto it = components.find(handle);
        if (it != components.end()) {
            return it->second.component;
        }
        return nullptr;
    }

    template<typename T>
    void EntityComponentSystem::AddComponent(const Handle &handle, std::shared_ptr<T> component) {
        auto &components = GetComponents<T>();
        components.insert({handle, { handle, std::move(component) }});
    }

    template<typename T>
    void EntityComponentSystem::RemoveComponent(Handle handle) {
        auto &components = GetComponents<T>();
        auto it = components.find(handle);
        if (it != components.end()) {
            components.erase(it);
        }
    }

}// namespace voxie
