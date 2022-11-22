//
// Created by Stefan Annell on 2021-05-12.
//

#pragma once
#include "Handle.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

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
            components[handle] = std::move(component);
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
        template<typename T>
        void RegisterComponent() {
            const char *typeName = typeid(T).name();

            assert(componentArrays.find(typeName) == componentArrays.end() && "Registering component type more than once.");

            componentArrays.insert({typeName, std::make_shared<ECSContainer<T>>()});
        }

        template<typename T>
        void Add(const Handle &handle, std::shared_ptr<T> component) {
            GetComponentArray<T>()->Add(handle, component);
        }

        template<typename T>
        void Remove(const Handle &handle) {
            GetComponentArray<T>()->Remove(handle);
        }

        template<typename T>
        std::shared_ptr<T> Get(const Handle &handle) {
            return GetComponentArray<T>()->Get(handle);
        }

        template<typename T>
        bool Has(const Handle &handle) {
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

    template<typename T>
    class ECSContainer2 : public ECSContainerInterface {
    public:
        void Add(const Handle &handle, const T &component) {
            usedSlots++;
            assert(!(usedSlots > slots) && "No more slots left.");

            auto &data = GetNextEmpty();
            data.handle = handle;
            data.data = component;
            data.active = true;
        }

        void Remove(Handle handle) {
            for (auto &obj : components) {
                if (obj.handle == handle) {
                    obj.active = false;
                }
            }
        }

        std::optional<std::reference_wrapper<T>> Get(const Handle &handle) {
            for (auto &obj : components) {
                if (obj.active && obj.handle == handle) {
                    return std::optional<std::reference_wrapper<T>>(obj.data);
                }
            }
            return std::nullopt;
        }

        bool Has(const Handle &handle) const {
            for (auto &obj : components) {
                if (obj.active && obj.handle == handle) {
                    return true;
                }
            }
            return false;
        }

    private:
        struct ContainerData {
            bool active = false;
            Handle handle = Handle(0);
            T data;
        };

        ContainerData &GetNextEmpty() {
            auto getNext = [&]() -> ContainerData & {
                if (currentSlot >= slots) {
                    currentSlot = 0;
                }
                return components[currentSlot++];
            };

            ContainerData &output = getNext();
            while (output.active) {
                output = getNext();
            }
            return output;
        }

        static constexpr size_t slots = 1000000;
        size_t usedSlots = 0;
        size_t currentSlot = 0;
        std::array<ContainerData, slots> components;
    };

    class ECSManagerArenaAllocator {
    private:
        struct Container {
            std::string name;
            std::unique_ptr<ECSContainerInterface> containerIF;
        };

    public:
        template<typename T>
        void
        RegisterComponent() {
            const char *typeName = typeid(T).name();
            assert(!GetContainerInterface(typeName) && "Registering component type more than once.");

            componentArrays[currentIndex++] = {typeName, std::make_unique<ECSContainer2<T>>()};
            assert(currentIndex < slots && "Out of slots!!");
        }

        template<typename T>
        void Add(const Handle &handle, const T &component) {
            GetComponentArray<T>()->Add(handle, component);
        }

        template<typename T>
        void Remove(const Handle &handle) {
            GetComponentArray<T>()->Remove(handle);
        }

        template<typename T>
        std::optional<std::reference_wrapper<T>> Get(const Handle &handle) {
            return GetComponentArray<T>()->Get(handle);
        }

        template<typename T>
        bool Has(const Handle &handle) {
            return GetComponentArray<T>()->Has(handle);
        }

    private:
        ECSContainerInterface *GetContainerInterface(const std::string &name) {
            for (const auto &container : componentArrays) {
                if (name == container.name) {
                    return container.containerIF.get();
                }
            }
            return nullptr;
        }
        template<typename T>
        ECSContainer2<T> *GetComponentArray() {
            auto *container = GetContainerInterface(typeid(T).name());
            assert(container && "Component not registered before use.");
            return static_cast<ECSContainer2<T> *>(container);
        }


        static constexpr size_t slots = 32;
        std::array<Container, slots> componentArrays;
        int currentIndex = 0;
    };

}// namespace voxie
