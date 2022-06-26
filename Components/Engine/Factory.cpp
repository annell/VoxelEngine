//
// Created by Stefan Annell on 2021-08-01.
//

#include "Factory.h"

#include "Camera.h"
#include "Chunk.h"
#include "Core.h"
#include "CubeEntity.h"
#include "PlayerController.h"
#include "Engine.h"
#include "Shader.h"
#include <Sprite.h>
#include <filesystem>

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace internal {

    void eraseSubStr(std::string &mainStr, const std::string &toErase) {
        // Search for the substring in string
        size_t pos = mainStr.find(toErase);
        if (pos != std::string::npos) {
            // If found then erase it from string
            mainStr.erase(pos, toErase.length());
        }
    }

    bool hasEnding(std::string const &fullString, std::string const &ending) {
        if (fullString.length() >= ending.length()) {
            return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
        } else {
            return false;
        }
    }

    template<typename T>
    std::vector<T> GetFiles(const std::string &pathToDir, std::optional<std::string> fileending = {}) {
        std::vector<T> output;
        voxie::Handle entity = voxie::Handle::MakeEntity();
        for (const auto &entry : std::filesystem::directory_iterator(pathToDir)) {
            std::string pathToFile = entry.path();
            std::string name = pathToFile;
            internal::eraseSubStr(name, pathToDir + "/");
            if (fileending.has_value() ? internal::hasEnding(name, fileending.value()) : true) {
                output.push_back(T{name, pathToFile, entity});
            }
        }
        return output;
    }
}// namespace internal

namespace voxie {

    std::vector<ModelConfig> GetModels() {
        return internal::GetFiles<ModelConfig>(BASE_PATH + MODELS, {MODELFILESENDING});
    }

    std::shared_ptr<voxie::Chunk> MakeModel(ModelConfig config) {
        auto model = std::make_shared<voxie::Chunk>(
                config.entity.GetId() ? config.entity : Handle::MakeEntity(),
                config.path,
                std::make_shared<voxie::Name>(config.name),
                std::make_shared<voxie::Shader>(
                        std::map<std::string, unsigned int>{
                                std::make_pair(BASE_PATH + SHADERS + "/cunk.vs", GL_VERTEX_SHADER),
                                std::make_pair(BASE_PATH + SHADERS + "/cunk.fs", GL_FRAGMENT_SHADER)}),
                std::make_shared<voxie::Position>(0, 0, 0));
        return std::move(model);
    }

    std::vector<BasePrimitives> GetBasePrimitives() {
        return {BasePrimitives::Cube};
    }

    std::shared_ptr<voxie::CubeEntity> MakePrimitive(PrimitivesConfig config) {
        auto model = std::make_shared<voxie::CubeEntity>(
                config.entity.GetId() ? config.entity : Handle::MakeEntity(),
                std::make_shared<voxie::Name>(config.name),
                std::make_shared<voxie::Shader>(
                        std::map<std::string, unsigned int>{
                                std::make_pair(BASE_PATH + SHADERS + "/cunk.vs", GL_VERTEX_SHADER),
                                std::make_pair(BASE_PATH + SHADERS + "/cunk.fs", GL_FRAGMENT_SHADER)}),
                std::make_shared<voxie::Position>(0, 0, 0),
                std::make_shared<voxie::Material>(config.material));
        return std::move(model);
    }

    std::vector<LightFactoryConfig> GetLights() {
        auto entity = Handle::MakeEntity();
        return {{
                        "Point light",
                        LightType::POINT,
                        entity,
                        {0, 0, 0},
                        {0.1, 0.1, 0.1},
                },
                {"Ambient light",
                 LightType::AMBIENT,
                 entity}};
    }

    std::shared_ptr<voxie::LightSource> MakeLight(LightFactoryConfig config) {
        auto obj = std::make_shared<voxie::LightSource>(
                voxie::LightConfig{
                        std::make_shared<voxie::Name>(config.name),
                        config.entity.GetId() ? config.entity : Handle::MakeEntity(),
                        std::make_shared<voxie::Position>(config.position),
                        config.type,
                        std::make_shared<voxie::Color>(config.color),
                        std::make_shared<voxie::Attenuation>(config.atteunation)});
        return std::move(obj);
    }

    std::shared_ptr<voxie::Camera> MakeCamera(const CameraFactoryConfig &config) {
        auto camera = std::make_shared<voxie::Camera>(
                config.entity.GetId() ? config.entity : Handle::MakeEntity(),
                config.name,
                config.position);
        return std::move(camera);
    }

    std::vector<SpriteConfig> GetSprites() {
        return internal::GetFiles<SpriteConfig>(BASE_PATH + SPRITES);
    }

    std::shared_ptr<voxie::Sprite> MakeSprite(SpriteConfig config) {
        auto sprite = std::make_shared<voxie::Sprite>(
                config.entity.GetId() ? config.entity : Handle::MakeEntity(),
                config.path,
                std::make_shared<voxie::Name>(config.name),
                std::make_shared<voxie::Shader>(
                        std::map<std::string, unsigned int>{
                                std::make_pair(BASE_PATH + SHADERS + "/sprite.vs", GL_VERTEX_SHADER),
                                std::make_pair(BASE_PATH + SHADERS + "/sprite.fs", GL_FRAGMENT_SHADER)}),
                std::make_shared<voxie::Position2D>(0, 0));
        return std::move(sprite);
    }

    std::shared_ptr<voxie::Text> MakeText(TextConfig config) {
        auto text = std::make_shared<voxie::Text>(
                config.entity.GetId() ? config.entity : Handle::MakeEntity(),
                std::make_shared<voxie::Name>(config.name),
                std::make_shared<voxie::Position2D>(0, 0),
                std::make_shared<voxie::Color>(config.color),
                std::make_shared<voxie::VisibleText>(config.text));
        return std::move(text);
    }

    std::shared_ptr<voxie::TransformNode> MakeTransformNode(TransformNodeConfig config) {
        auto sprite = std::make_shared<voxie::TransformNode>(
                config.entity.GetId() ? config.entity : Handle::MakeEntity(),
                std::make_shared<voxie::Name>(config.name),
                std::make_shared<voxie::Position>(0, 0, 0));
        return std::move(sprite);
    }

    std::vector<SceneConfig> GetScenes() {
        return internal::GetFiles<SceneConfig>(BASE_PATH + SCENES, {SCENEFILESENDING});
    }

    std::shared_ptr<PlayerController> MakePlayerController(const PlayerControllerFactoryConfig &config) {
        auto playerController = std::make_shared<PlayerController>(
                config.entity.GetId() ? config.entity : Handle::MakeEntity(),
                std::make_shared<voxie::Name>(config.name),
                std::make_shared<voxie::Position>(0, 0, 0));
        return std::move(playerController);
    }

}// namespace voxie
