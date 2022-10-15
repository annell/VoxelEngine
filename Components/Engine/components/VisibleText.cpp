//
// Created by Stefan Annell on 2022-01-23.
//

#include "VisibleText.h"
#include <yaml-cpp/yaml.h>

namespace voxie {
    VisibleText::VisibleText() {
    }

    VisibleText::VisibleText(std::string text)
        : text(text) {
    }
}// namespace voxie
