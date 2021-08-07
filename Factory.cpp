//
// Created by Stefan Annell on 2021-08-01.
//

#include "Factory.h"

#include "Core.h"
#include "Engine.h"
#include "Chunk.h"

namespace voxie
{

std::vector<ModelConfig> GetModels() {
    return {
            {"Girl", "/chr_knight.vox"},
            {"Boy", "/chr_sword.vox"},
            {"Floor", "/floor.vox"}
    };
}

std::shared_ptr<voxie::Chunk> MakeModel(ModelConfig config) {
    auto model = std::make_shared<voxie::Chunk>(
            BASE_PATH + MODELS + config.path,
            std::make_shared<voxie::Name>(config.name),
            std::make_shared<voxie::Shader>(
            std::map<std::string, unsigned int>{
                    std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
                    std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
            }), std::make_shared<voxie::Position>(0, 0, 0));
    voxie::Engine::GetEngine().GetScene().AddEntity(model->GetEntity());
    return std::move(model);
}


std::vector<LightFactoryConfig> GetLights() {
    return {{
        "Point light",
         LightType::POINT,
         {0, 0, 0},
         {0.1, 0.1, 0.1},
         }, {
        "Ambient light",
        LightType::AMBIENT,

    }
    };
}

std::shared_ptr<voxie::LightSource> MakeLight(LightFactoryConfig config) {
    auto obj = std::make_shared<voxie::LightSource>(
            voxie::LightConfig{
                    std::make_shared<voxie::Name>(config.name),
                    voxie::Entity::MakeEntity(),
                    std::make_shared<voxie::Shader>(std::map<std::string, unsigned int>{
                            std::make_pair(voxie::BASE_PATH + voxie::SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                            std::make_pair(voxie::BASE_PATH + voxie::SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
                    }),
                    std::make_shared<voxie::Position>(config.position),
                    config.type,
                    std::make_shared<voxie::Cube>(voxie::Position{0, 0, 0}, config.dimensions),
                    std::make_shared<voxie::Color>(config.color),
                    std::make_shared<voxie::Atteunation>(config.atteunation)
            });
    voxie::Engine::GetEngine().GetScene().AddEntity(obj->GetEntity());
    return std::move(obj);
}

}
