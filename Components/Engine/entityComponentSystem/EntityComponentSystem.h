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

        std::unordered_map<Handle, std::shared_ptr<T>> &GetContainer() {
            return components;
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

        template<typename T>
        std::unordered_map<Handle, std::shared_ptr<T>> &GetArray() {
            return GetComponentArray<T>()->GetContainer();
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

        static constexpr size_t slots = 1024;
        struct ContainerData {
            bool active = false;
            Handle handle = Handle(0);
            T data;
        };

        std::array<ContainerData, slots> &GetContainer() {
            return components;
        }

    private:
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

        size_t usedSlots = 0;
        size_t currentSlot = 0;
        std::array<ContainerData, slots> components;
    };

    class ECSManagerArenaAllocator {
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

        template<typename T>
        std::array<typename ECSContainer2<T>::ContainerData, ECSContainer2<T>::slots> &GetArray() {
            return GetComponentArray<T>()->GetContainer();
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

        struct Container {
            std::string name;
            std::unique_ptr<ECSContainerInterface> containerIF;
        };


        static constexpr size_t slots = 32;
        std::array<Container, slots> componentArrays;
        int currentIndex = 0;
    };

    template<typename... Ti>
    struct Iterator {
        using value_type = std::tuple<Ti...>;

        Iterator(size_t slot, size_t usedSlots) : slot(slot), usedSlots(usedSlots) {}

        value_type &operator*() const { return *m_ptr; }
        value_type *operator->() { return m_ptr; }
        Iterator &operator++() {
            slot++;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend bool operator==(const Iterator &a, const Iterator &b) { return a.slot == b.slot; };
        friend bool operator!=(const Iterator &a, const Iterator &b) { return a.slot != b.slot; };

    private:
        size_t slot = 0;
        const size_t usedSlots = 0;
        value_type *m_ptr = nullptr;
    };

    template<typename TypeToCheck, typename... TypesToCheckAgainst>
    concept type_in = (std::same_as<std::remove_cvref_t<TypeToCheck>, TypesToCheckAgainst> || ...);

    template<typename... T>
    class ECSManagerConcepts {
    public:
        ECSManagerConcepts() : componentArrays({}), ComponentTs({}), activeSlots({}) {
        }

        template<typename TData>
        requires type_in<TData, T...>
        void Add(const Handle &handle, const TData &component) {
            usedSlots++;
            assert(!(usedSlots > slots) && "No more slots left.");

            auto &data = GetNextEmpty<TData>();
            data.handle = handle;
            data.data = component;
            data.active = true;
        }

        template<typename TData>
        requires type_in<TData, T...>
        void Remove(const Handle &handle) {
            for (auto &obj : GetArray<TData>()) {
                if (obj.handle == handle) {
                    if (obj.active == true) {
                        currentSlot--;
                        obj.active = false;
                        return;
                    }
                }
            }
        }

        template<typename TData>
        requires type_in<TData, T...>
        [[nodiscard]] TData &Get(const Handle &handle) {
            for (auto &obj : GetArray<TData>()) {
                if (obj.active && obj.handle == handle) {
                    return obj.data;
                }
            }
            throw std::invalid_argument("Handle not present");
            return GetArray<TData>()[0].data;
        }

        template<typename TData>
        requires type_in<TData, T...>
        [[nodiscard]] bool Has(const Handle &handle) {
            for (auto &obj : GetArray<TData>()) {
                if (obj.active && obj.handle == handle) {
                    return true;
                }
            }
            return false;
        }

        template<typename TData>
        requires type_in<TData, T...>
        struct ContainerData {
            bool active = false;
            Handle handle = Handle(0);
            TData data;
        };

        struct EntitySlots {
            template<typename Component>
            struct ComponentActive { bool active = false; };
            using ComponentsActive = std::tuple<ComponentActive<T>...>;

            ComponentsActive IsActive{};
            bool isEntityActive = false;
            Handle handle = Handle(0);
            size_t slot = 0;
        };

        std::tuple<T...> ComponentTs;

        static constexpr size_t slots = 1024;
        template<typename TData>
        using ComponentArray = std::array<ContainerData<TData>, slots>;
        using ComponentArrays = std::tuple<ComponentArray<T>...>;

        using EntitiesSlots = std::array<EntitySlots, slots>;

        template<typename TData>
        requires type_in<TData, T...>
        [[nodiscard]] ComponentArray<TData>
                &GetArray() {
            return std::get<ComponentArray<TData>>(componentArrays);
        }

        const EntitiesSlots &GetEntitiesSlots() const {
            return activeSlots;
        }

    private:
        template<typename TData>
        [[nodiscard]] ContainerData<TData> &GetNextEmpty() {
            auto getNext = [&]() -> ContainerData<TData> & {
                if (currentSlot >= slots) {
                    currentSlot = 0;
                }
                return GetArray<TData>()[currentSlot++];
            };

            auto &output = getNext();
            while (output.active) {
                output = getNext();
            }
            return output;
        }

        ComponentArrays componentArrays{};

        size_t usedSlots = 0;
        size_t currentSlot = 0;

        EntitiesSlots activeSlots;
    };


    template<typename... Ti>
    struct Filter {

        Filter(auto &ecs) : components(std::tie(ecs.template GetArray<Ti>()...)) {
        }
        [[nodiscard]] Iterator<Ti...> begin() { return Iterator<Ti...>(0, 0); }

        [[nodiscard]] Iterator<Ti...> end() { return Iterator<Ti...>(0, 0); }

    private:
        typename ECSManagerConcepts<Ti...>::ComponentArrays components;
    };
}// namespace voxie
