//
// Created by Stefan Annell on 2021-08-01.
//

#include "Factory.h"

#include "Core.h"
#include "ModelLoader.h"
#include "Engine.h"
#include "Cube.h"

namespace voxie
{


std::vector<ModelConfig> GetModels() {
    return {
            {"Girl", "/chr_knight.vox"},
            {"Boy", "/chr_sword.vox"},
            {"Floor", "/floor.vox"}
    };
}


std::unique_ptr<voxie::Chunk> MakeModel(ModelConfig config) {
    auto model = std::make_unique<voxie::Chunk>(config.name, std::make_shared<voxie::Shader>(
            std::map<std::string, unsigned int>{
                    std::make_pair(BASE_PATH + SHADERS + "/basic_light.vs", GL_VERTEX_SHADER),
                    std::make_pair(BASE_PATH + SHADERS + "/basic_light.fs", GL_FRAGMENT_SHADER)
            }), std::make_shared<voxie::Position>(0, 0, 0));
    if (!config.path.empty()) {
        voxie::ModelLoader::LoadModel(BASE_PATH + MODELS + config.path, model.get());
    }
    model->Init();
    auto& engine = voxie::Engine::GetEngine();
    engine.GetScene().AddEntity(model->GetEntity());
    engine.GetLightSourceHandler().AddShader(model->GetShader());
    return std::move(model);
}

std::unique_ptr<voxie::LightSource> MakeLight(std::string name,
                                              voxie::LightType type,
                                              voxie::Position position,
                                              voxie::Dimensions dimensions,
                                              voxie::Color color,
                                              voxie::Atteunation atteunation) {
    return std::make_unique<voxie::LightSource>(
            voxie::LightConfig{
                    std::make_shared<std::string>(name),
                    voxie::Entity::MakeEntity(),
                    std::make_shared<voxie::Shader>(std::map<std::string, unsigned int>{
                            std::make_pair(voxie::BASE_PATH + voxie::SHADERS + "/light_cube.vs", GL_VERTEX_SHADER),
                            std::make_pair(voxie::BASE_PATH + voxie::SHADERS + "/light_cube.fs", GL_FRAGMENT_SHADER)
                    }),
                    std::make_shared<voxie::Position>(position),
                    type,
                    std::make_shared<voxie::Cube>(voxie::Position{0, 0, 0}, dimensions),
                    std::make_shared<voxie::Color>(color),
                    std::make_shared<voxie::Atteunation>(atteunation)
            });
}

}
