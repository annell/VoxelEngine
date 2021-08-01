//
// Created by Stefan Annell on 2021-08-01.
//

#include "ModelHandler.h"

void ModelHandler::AddModel(std::unique_ptr <voxie::Chunk>&& model) {
    models.push_back(std::move(model));
}

std::vector<voxie::RenderingConfig> ModelHandler::GetRenderingConfigs() const {
    std::vector<voxie::RenderingConfig> configs;
    for (auto& model : models) {
        configs.push_back(model->GetRenderingConfig());
    }
    return configs;
}
