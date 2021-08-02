//
// Created by Stefan Annell on 2021-05-12.
//

#pragma once
#include <map>
#include <string>

namespace voxie {

class Entity {
public:
    using Identity = int;
    using Type = int;
    Entity(Identity, Type);
    static std::shared_ptr<Entity> MakeEntity();
    const Identity& GetId() const;

    bool operator<(const Entity& rhs) const;

private:
    auto as_tie() const;

    const Entity::Identity id;
    Type type;
};

class EntityComponentSystem {
public:
    template <typename T>
    std::shared_ptr<T> GetComponent(const Entity& handle);

    template <typename T>
    void AddComponent(std::shared_ptr<T> component, const Entity& handle);
private:
};

template <typename T>
std::map<Entity, std::shared_ptr<T>> &GetComponents() {
    static std::map<Entity, std::shared_ptr<T>> entities;
    return entities;
}

template<typename T>
std::shared_ptr<T> EntityComponentSystem::GetComponent(const Entity& handle) {
    auto& components = GetComponents<T>();
    auto it = components.find(handle);
    if (it != components.end()) {
        return it->second;
    }
    return nullptr;
}

template<typename T>
void EntityComponentSystem::AddComponent(std::shared_ptr<T> component, const Entity& handle) {
    GetComponents<T>()[handle] = component;
}

}
