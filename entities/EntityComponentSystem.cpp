//
// Created by Stefan Annell on 2021-05-12.
//

#include "EntityComponentSystem.h"

namespace engine::entities {

Entity::Entity(Entity::Identity id, std::string name, Type type)
 : id(id), name(name), type(type) {
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

std::shared_ptr<Entity> Entity::MakeEntity(std::string name) {
    static int id = 0;
    return std::make_shared<Entity>(id++, name, 0);
}

const std::string &Entity::GetName() const {
    return name;
}

}
