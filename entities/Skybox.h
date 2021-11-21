//
// Created by Stefan Annell on 2021-11-21.
//

#pragma once

#include "Cube.h"
#include "EntityComponentSystem.h"
#include "RenderingHandler.h"

namespace voxie {

    class Skybox {
    public:
        Skybox(const Handle &, std::shared_ptr<Shader>);
        [[nodiscard]] RenderingConfig GetRenderingConfig() const;
        [[nodiscard]] std::shared_ptr<Shader> GetShader() const;
        [[nodiscard]] std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;

    private:
        void Init();
        auto GetPostDrawAction() const;
        auto GetPreDrawAction(const std::shared_ptr<Shader> &shader, const std::shared_ptr<Camera> &camera) const;

        unsigned int cubemapTexture = 0;
        Cube cube;
        Handle handle;
    };

}// namespace voxie
