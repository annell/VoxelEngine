//
// Created by Stefan Annell on 2021-05-17.
//

#pragma once
#include "Attenuation.h"
#include "Color.h"
#include "Cube.h"
#include "Direction.h"
#include "Material.h"
#include "Name.h"
#include "Position.h"
#include "Position2D.h"
#include "Skybox.h"
#include "VisibleText.h"
#include "RigidBody.h"

namespace YAML {
    template<typename T>
    struct convert {
        static Node encode(const T &rhs) {
            Node node;
            rhs.encode(node);
            return node;
        }

        static bool decode(const Node &node, T &rhs) {
            return rhs.decode(node);
        }
    };
}// namespace YAML
