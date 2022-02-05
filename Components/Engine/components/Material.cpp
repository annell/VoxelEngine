//
// Created by Stefan Annell on 2022-01-11.
//

#include "Material.h"
#include <yaml-cpp/yaml.h>

namespace voxie {

void Material::encode(YAML::Node &node) const {
    YAML::Node ambientNode;
    ambientNode["x"] = ambient[0];
    ambientNode["y"] = ambient[1];
    ambientNode["z"] = ambient[2];
    node["ambient"] = ambientNode;

    YAML::Node diffuseNode;
    diffuseNode["x"] = diffuse[0];
    diffuseNode["y"] = diffuse[1];
    diffuseNode["z"] = diffuse[2];
    node["diffuse"] = diffuseNode;

    YAML::Node specularNode;
    specularNode["x"] = specular[0];
    specularNode["y"] = specular[1];
    specularNode["z"] = specular[2];
    node["specular"] = specularNode;

    node["shininess"] = shininess;
}

bool Material::decode(const YAML::Node &node) {
    auto root = node["ambient"];
    ambient = {root["x"].as<float>(),
               root["y"].as<float>(),
               root["z"].as<float>()};

    root = node["diffuse"];
    diffuse = {root["x"].as<float>(),
               root["y"].as<float>(),
               root["z"].as<float>()};

    root = node["specular"];
    diffuse = {root["x"].as<float>(),
               root["y"].as<float>(),
               root["z"].as<float>()};

    shininess = node["shininess"].as<float>();

    return true;
}

}
