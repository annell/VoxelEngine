//
// Created by Stefan Annell on 2021-08-01.
//

#pragma once

#include <map>
#include <memory>
#include "Chunk.h"

class ModelHandler {
public:
    ModelHandler() = default;
    void AddModel(std::unique_ptr<voxie::Chunk>&&);
    void RemoveModel(voxie::Entity entity);
    std::vector<voxie::RenderingConfig> GetRenderingConfigs() const;

private:
    std::map<voxie::Entity, std::unique_ptr<voxie::Chunk>> models;
};


