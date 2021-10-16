//
// Created by Stefan Annell on 2021-05-12.
//

#include "EntityComponentSystem.h"

namespace voxie {

    Entity::Entity(Entity::Identity id)
        : id(id) {
    }

    auto Entity::as_tie() const {
        return std::tie(id);
    }

    bool Entity::operator<(const Entity &rhs) const {
        return as_tie() < rhs.as_tie();
    }

    bool Entity::operator==(const Entity &rhs) const {
        return as_tie() == rhs.as_tie();
    }

    Entity Entity::MakeEntity() {
        static int id = 0;
        return Entity(++id);
    }
    Entity::Identity Entity::GetId() const {
        return id;
    }

}// namespace voxie
