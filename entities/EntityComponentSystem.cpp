//
// Created by Stefan Annell on 2021-05-12.
//

#include "EntityComponentSystem.h"

namespace voxie {

Entity::Entity(Entity::Identity id, Type type)
 : id(id), type(type) {
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

std::shared_ptr<Entity> Entity::MakeEntity() {
    static int id = 0;
    return std::make_shared<Entity>(id++, 0);
}

}
