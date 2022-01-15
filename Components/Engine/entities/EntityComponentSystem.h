//
// Created by Stefan Annell on 2021-05-12.
//

#pragma once
#include <algorithm>
#include <vector>
#include <string>

namespace voxie {

    class Handle {
    public:
        using Identity = int;
        explicit Handle(Identity);

        static Handle MakeEntity();

        Handle::Identity GetId() const;

        bool operator<(const Handle &rhs) const;

        bool operator==(const Handle &rhs) const;

    private:
        [[nodiscard]] auto as_tie() const;

        Handle::Identity id;
    };
    static const Handle NullEntity(0);

    class EntityComponentSystem {
    public:
        template<typename T>
        std::shared_ptr<T> GetComponent(const Handle &handle);

        template<typename T>
        void AddComponent(const Handle &handle, std::shared_ptr<T> component);

        template<typename T>
        void RemoveComponent(const Handle &handle);

    private:
    };

    template<typename T>
    struct Component {
        Handle handle;
        std::shared_ptr<T> component;

        bool operator==(const Component<T>& rhs) {
            return this->handle == rhs.handle;
        }
    };

    template<typename T>
    std::vector<Component<T>> &GetComponents() {
        static std::vector<Component<T>> components;
        return components;
    }

    template<typename T>
    std::shared_ptr<T> EntityComponentSystem::GetComponent(const Handle &handle) {
        auto &components = GetComponents<T>();
        for (auto it = components.begin(); it != components.end(); it++) {
            if (it->handle == handle) {
                return it->component;
            }
        }
        return nullptr;
    }

    template<typename T>
    void EntityComponentSystem::AddComponent(const Handle &handle, std::shared_ptr<T> component) {
        auto &components = GetComponents<T>();
        components.push_back({handle, std::move(component)});
    }

    template<typename T>
    void EntityComponentSystem::RemoveComponent(const Handle &handle) {
        auto &components = GetComponents<T>();
        for (auto it = components.begin(); it != components.end(); it++) {
            if (it->handle == handle) {
                components.erase(it);
                return;
            }
        }
    }

}// namespace voxie
