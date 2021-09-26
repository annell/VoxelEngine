//
// Created by Stefan Annell on 2021-08-01.
//

#include "Factory.h"

#include "Camera.h"
#include "Chunk.h"
#include "Core.h"
#include "Engine.h"
#include <Sprite.h>
#include <filesystem>

namespace internal {

    void eraseSubStr(std::string &mainStr, const std::string &toErase) {
        // Search for the substring in string
        size_t pos = mainStr.find(toErase);
        if (pos != std::string::npos) {
            // If found then erase it from string
            mainStr.erase(pos, toErase.length());
        }
    }

    bool hasEnding (std::string const &fullString, std::string const &ending) {
        if (fullString.length() >= ending.length()) {
            return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
        } else {
            return false;
        }
    }
}// namespace internal

namespace voxie {

    std::vector<ModelConfig> GetModels() {
        std::vector<ModelConfig> output;
        for (const auto &entry : std::filesystem::directory_iterator(BASE_PATH + MODELS)) {
            std::string path = entry.path();
            std::string name = path;
            internal::eraseSubStr(name, BASE_PATH + MODELS);
            output.push_back(ModelConfig{name, path});
        }
        return output;
    }

    std::shared_ptr<voxie::Chunk> MakeModel(ModelConfig config) {
        auto model = std::make_shared<voxie::Chunk>(
                config.path,
                std::make_shared<voxie::Name>(config.name),
                std::make_shared<voxie::Shader>(
                        std::map<std::string, unsigned int>{
                                std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
                                std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)}),
                std::make_shared<voxie::Position>(0, 0, 0));
        voxie::Engine::GetEngine().GetScene().AddEntity(model->GetEntity());
        return std::move(model);
    }


    std::vector<LightFactoryConfig> GetLights() {
        return {{
                        "Point light",
                        LightType::POINT,
                        {0, 0, 0},
                        {0.1, 0.1, 0.1},
                },
                {
                        "Ambient light",
                        LightType::AMBIENT,

                }};
    }

    std::shared_ptr<voxie::LightSource> MakeLight(LightFactoryConfig config) {
        auto obj = std::make_shared<voxie::LightSource>(
                voxie::LightConfig{
                        std::make_shared<voxie::Name>(config.name),
                        voxie::Entity::MakeEntity(),
                        std::make_shared<voxie::Shader>(std::map<std::string, unsigned int>{
                                std::make_pair(voxie::BASE_PATH + voxie::SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                                std::make_pair(voxie::BASE_PATH + voxie::SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)}),
                        std::make_shared<voxie::Position>(config.position),
                        config.type,
                        std::make_shared<voxie::Cube>(voxie::Position{0, 0, 0}, config.dimensions),
                        std::make_shared<voxie::Color>(config.color),
                        std::make_shared<voxie::Attenuation>(config.atteunation)});
        voxie::Engine::GetEngine().GetScene().AddEntity(obj->GetEntity());
        return std::move(obj);
    }

    std::shared_ptr<voxie::Camera> MakeCamera(const CameraFactoryConfig& config) {
        auto camera = std::make_shared<voxie::Camera>(voxie::Entity::MakeEntity(), config.name, config.position);
        voxie::Engine::GetEngine().GetScene().AddEntity(camera->GetEntity());
        return std::move(camera);
    }

    std::vector<SpriteConfig> GetSprites() {
        std::vector<SpriteConfig> output;
        for (const auto &entry : std::filesystem::directory_iterator(BASE_PATH + SPRITES)) {
            std::string path = entry.path();
            std::string name = path;
            internal::eraseSubStr(name, BASE_PATH + SPRITES);
            output.push_back(SpriteConfig{name, path});
        }
        return output;
    }

    std::shared_ptr<voxie::Sprite> MakeSprite(SpriteConfig config) {
        auto sprite = std::make_shared<voxie::Sprite>(
                config.path,
                std::make_shared<voxie::Name>(config.name),
                std::make_shared<voxie::Shader>(
                        std::map<std::string, unsigned int>{
                                std::make_pair(BASE_PATH + SHADERS + "/sprite.vs", GL_VERTEX_SHADER),
                                std::make_pair(BASE_PATH + SHADERS + "/sprite.fs", GL_FRAGMENT_SHADER)}),
                std::make_shared<voxie::Position2D>(0, 0));
        voxie::Engine::GetEngine().GetScene().AddEntity(sprite->GetEntity());
        return std::move(sprite);
    }

    std::vector<SceneConfig> GetScenes() {
        std::vector<SceneConfig> output;
        for (const auto &entry : std::filesystem::directory_iterator(BASE_PATH + SCENES)) {
            std::string path = entry.path();
            std::string name = path;
            internal::eraseSubStr(name, BASE_PATH + SCENES + "/");
            if (internal::hasEnding(name, SCENEFILESENDING)) {
                output.push_back(SceneConfig{name, path});
            }
        }
        return output;
    }

}// namespace voxie
