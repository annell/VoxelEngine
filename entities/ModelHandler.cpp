//
// Created by Stefan Annell on 2021-08-01.
//

#include "ModelHandler.h"

void ModelHandler::AddModel(std::unique_ptr <voxie::Chunk>&& model) {
    models[*model->GetEntity()] = std::move(model);
}

void ModelHandler::RemoveModel(voxie::Entity entity) {
    auto it = models.find(entity);
    if (it != models.end()) {
        models.erase(it);
    }
}

std::vector<voxie::RenderingConfig> ModelHandler::GetRenderingConfigs() const {
    std::vector<voxie::RenderingConfig> configs;
    for (auto& pair : models) {
        auto& model = pair.second;
        configs.push_back(model->GetRenderingConfig());
    }
    return configs;
}
