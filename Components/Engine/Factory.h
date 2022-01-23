//
// Created by Stefan Annell on 2021-08-01.
//
#pragma once

#include <TransformNode.h>
#include <Lightsource.h>
#include <Sprite.h>
#include "Text.h"
#include <string>
#include <filesystem>
#include <memory>

#include "Cube.h"
#include "BaseComponents.h"

namespace voxie {

    class Chunk;
    class CubeEntity;

    const std::string BASE_PATH = std::filesystem::current_path().string() + std::string("/resources");
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
    std::shared_ptr<Chunk> MakeModel(ModelConfig);

    enum class BasePrimitives {
        Cube,
        Unknown,
    };

    struct PrimitivesConfig {
        std::string name;
        BasePrimitives primitive = BasePrimitives::Unknown;
        Material material = {
                {0.2f, 0.8f, 0.3f},
                {0.5, 0.5, 0.5},
                {1.0f, 0.5f, 0.31f},
                {0.5f, 0.5f, 0.5f},
                32.0f};
        Handle entity = NullEntity;
    };

    std::vector<BasePrimitives> GetBasePrimitives();
    std::shared_ptr<CubeEntity> MakePrimitive(PrimitivesConfig);

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

    struct TextConfig {
        std::string name;
        std::string text;
        voxie::Color color = voxie::Color({0.5, 0.5, 0.5});
        Handle entity = NullEntity;
    };

    std::shared_ptr<voxie::Text> MakeText(TextConfig);

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