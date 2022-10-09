//
// Created by Stefan Annell on 2021-05-17.
//

#pragma once
#include "Name.h"

#include "Material.h"

#include "Color.h"

#include "Attenuation.h"

#include "Position.h"

#include "Position2D.h"

#include "Direction.h"

#include "Skybox.h"

#include "Cube.h"

#include "VisibleText.h"

#include "RigidBody.h"

#include "Outline.h"

#include "Verlet.h"

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
