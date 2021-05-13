//
// Created by Stefan Annell on 2021-05-12.
//

#include "EntityComponentSystem.h"

Entity::Entity(Entity::Identity id)
 : id(id) {
}

const Entity::Identity &Entity::GetId() const {
    return id;
}

auto Entity::as_tie() const {
    return std::tie(id);
}

bool Entity::operator<(const Entity &rhs) const {
    return as_tie() < rhs.as_tie();
}
