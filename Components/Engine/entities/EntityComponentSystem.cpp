//
// Created by Stefan Annell on 2021-05-12.
//

#include "EntityComponentSystem.h"

namespace voxie {

    Handle::Handle(Handle::Identity id)
        : id(id) {
    }

    auto Handle::as_tie() const {
        return std::tie(id);
    }

    bool Handle::operator<(const Handle &rhs) const {
        return as_tie() < rhs.as_tie();
    }

    bool Handle::operator==(const Handle &rhs) const {
        return as_tie() == rhs.as_tie();
    }

    Handle Handle::MakeEntity() {
        static int id = 0;
        return Handle(++id);
    }
    Handle::Identity Handle::GetId() const {
        return id;
    }

}// namespace voxie
