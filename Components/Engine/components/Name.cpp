//
// Created by Stefan Annell on 2022-01-11.
//

#include "Name.h"
#include <yaml-cpp/yaml.h>

namespace voxie {
    Name::Name() : Name("") {
    }
    Name::Name(std::string name)
        : name(std::move(name)) {
    }
}// namespace voxie
