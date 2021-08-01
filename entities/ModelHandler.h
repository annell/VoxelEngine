//
// Created by Stefan Annell on 2021-08-01.
//

#pragma once

#include <vector>
#include <memory>
#include "Chunk.h"

class ModelHandler {
public:
    ModelHandler() = default;
    void AddModel(std::unique_ptr<voxie::Chunk>&&);
    std::vector<voxie::RenderingConfig> GetRenderingConfigs() const;
private:
    std::vector<std::unique_ptr<voxie::Chunk>> models;
};


