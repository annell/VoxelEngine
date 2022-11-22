//
// Created by Stefan Annell on 2022-01-23.
//

#include "Handle.h"
#include <random>

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
        return id == rhs.id;
    }

    Handle Handle::MakeEntity() {
        static std::random_device rd;
        static std::mt19937_64 e2(rd());
        static std::uniform_int_distribution<long long int> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));

        return Handle(dist(e2));
    }
    Handle::Identity Handle::GetId() const {
        return id;
    }

}// namespace voxie
