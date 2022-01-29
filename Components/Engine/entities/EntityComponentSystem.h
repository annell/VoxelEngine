//
// Created by Stefan Annell on 2021-05-12.
//

#pragma once
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>
#include "Handle.h"

namespace voxie {

    static const Handle NullEntity(0);

    using ComponentType = std::uint8_t;
    const ComponentType MAX_COMPONENTS = 32;

    class ECSContainerInterface {
    public:
        virtual ~ECSContainerInterface() = default;

        virtual void Remove(Handle handle) = 0;

    };

    template<typename T>
    class ECSContainer : public ECSContainerInterface {
    public:

        void Add(const Handle &handle, std::shared_ptr<T> component) {
            components.insert({handle, std::move(component)});
        }

        void Remove(Handle handle) override {
            auto it = components.find(handle);
            if (it != components.end()) {
                components.erase(it);
            } else {
                assert(!"Component not found, could not remove.");
            }
        }

        std::shared_ptr<T> Get(const Handle &handle) {
            auto it = components.find(handle);
            if (it != components.end()) {
                return it->second;
            }
            return nullptr;
        }

    private:
        std::unordered_map<Handle, std::shared_ptr<T>> components;
    };



    class ECSManager {
    public:
        template <typename T>
        void RegisterComponent() {
            const char* typeName = typeid(T).name();

            assert(componentArrays.find(typeName) == componentArrays.end() && "Registering component type more than once.");

            componentArrays.insert({typeName, std::make_shared<ECSContainer<T>>()});
        }

        template<typename T>
        void Add(const Handle& handle, std::shared_ptr<T> component) {
            GetComponentArray<T>()->Add(handle, component);
        }

        template<typename T>
        void Remove(const Handle& handle) {
            GetComponentArray<T>()->Remove(handle);
        }

        template<typename T>
        std::shared_ptr<T> Get(const Handle& handle) {
            return GetComponentArray<T>()->Get(handle);
        }

        template<typename T>
        bool Has(const Handle& handle) {
            return Get<T>(handle) != nullptr;
        }


    private:
        template<typename T>
        std::shared_ptr<ECSContainer<T>> GetComponentArray() {
            std::string typeName = typeid(T).name();

            auto it = componentArrays.find(typeName);
            assert(it != componentArrays.end() && "Component not registered before use.");
            return std::static_pointer_cast<ECSContainer<T>>(it->second);
        }

        std::unordered_map<std::string, std::shared_ptr<ECSContainerInterface>> componentArrays;
    };

}// namespace voxie
