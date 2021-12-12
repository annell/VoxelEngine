//
// Created by Stefan Annell on 2021-08-01.
//
#pragma once

#include <Chunk.h>
#include <TransformNode.h>
#include <Lightsource.h>
#include <Sprite.h>
#include <string>

namespace voxie {

    //const std::string BASE_PATH = std::filesystem::current_path().string() + std::string("/resources");
    const std::string BASE_PATH = "/Users/stan/dev/C++/VoxelEngine/Components/Engine/resources";

    const std::string SHADERS = "/shaders";
    const std::string FONTS = "/fonts";
    const std::string MODELS = "/voxelObjects";
    const std::string SPRITES = "/sprites";
    const std::string SCENES = "/scenes";
    const std::string SCENEFILESENDING = ".voxie";
    const std::string MODELFILESENDING = ".vox";

    struct ModelConfig {
        std::string name;
        std::string path;
        Handle entity = NullEntity;
    };

    std::vector<ModelConfig> GetModels();
    std::shared_ptr<voxie::Chunk> MakeModel(ModelConfig);

    struct LightFactoryConfig {
        std::string name;
        voxie::LightType type;
        Handle entity = NullEntity;
        voxie::Position position = {0, 0, 0};
        voxie::Dimensions dimensions = {1, 1, 1};
        voxie::Color color = voxie::Color({0.5, 0.5, 0.5});
        voxie::Attenuation atteunation = {1, 1, 1};
    };

    std::vector<LightFactoryConfig> GetLights();
    std::shared_ptr<voxie::LightSource> MakeLight(LightFactoryConfig);

    struct CameraFactoryConfig {
        std::string name;
        Handle entity = NullEntity;
        glm::vec3 position = {0, 0, 0};
    };

    std::shared_ptr<voxie::Camera> MakeCamera(const CameraFactoryConfig &);

    struct SpriteConfig {
        std::string name;
        std::string path;
        Handle entity = NullEntity;
    };

    std::vector<SpriteConfig> GetSprites();
    std::shared_ptr<voxie::Sprite> MakeSprite(SpriteConfig);

    struct TransformNodeConfig {
        std::string name;
        Handle entity = NullEntity;
    };

    std::shared_ptr<voxie::TransformNode> MakeTransformNode(TransformNodeConfig);

    struct SceneConfig {
        std::string name;
        std::string path;
        Handle entity = NullEntity;
    };
    std::vector<SceneConfig> GetScenes();

}// namespace voxie