//
// Created by Stefan Annell on 2022-01-11.
//

#include "Material.h"
#include <yaml-cpp/yaml.h>

namespace voxie {
    Material::Material() : Material({}, {}, {}, 0.0f) {
    }
    Material::Material(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {
    }
    Material::Material(const Material &material)
        : ambient(material.ambient), diffuse(material.diffuse), specular(material.specular), shininess(material.shininess) {
    }

}// namespace voxie
