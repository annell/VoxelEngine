//
// Created by Stefan Annell on 2021-08-01.
//
#pragma once

#include <string>
#include <Lightsource.h>
#include <Chunk.h>

namespace voxie {

const std::string BASE_PATH = "/Users/stan/dev/C++/VoxelEngine/resources";
const std::string SHADERS = "/shaders";
const std::string FONTS = "/fonts";
const std::string MODELS = "/voxelObjects";

struct ModelConfig {
    std::string name;
    std::string path;
};

std::vector<ModelConfig> GetModels();
std::shared_ptr<voxie::Chunk> MakeModel(ModelConfig);

struct LightFactoryConfig {
    std::string name;
    voxie::LightType type;
    voxie::Position position = {0, 0, 0};
    voxie::Dimensions dimensions = {1, 1, 1};
    voxie::Color color = glm::vec3{0.5, 0.5, 0.5};
    voxie::Atteunation atteunation = {10, 10, 10};
};

std::vector<LightFactoryConfig> GetLights();
std::shared_ptr<voxie::LightSource> MakeLight(LightFactoryConfig);

struct CameraFactoryConfig {
    std::string name;
    glm::vec3 position = {0, 0, 0};
};

std::shared_ptr<voxie::Camera> MakeCamera(CameraFactoryConfig);

}