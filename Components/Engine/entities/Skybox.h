//
// Created by Stefan Annell on 2021-11-21.
//

#pragma once

#include "Cube.h"
#include "EntityComponentSystem.h"
#include "Node.h"
#include "RenderingHandler.h"

namespace voxie {
    class Shader;
    class VertexBufferArray;
    class Camera;

    class Skybox : public NodeWrapper {
    public:
        Skybox(const Handle &, std::shared_ptr<Shader>);

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);
        COMPONENT_ADD_FUNCTION(Shader);

        RenderingConfig GetRenderingConfig() const override;
        std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;

    private:
        void Init();
        auto GetPostDrawAction() const;
        auto GetPreDrawAction(const std::shared_ptr<Shader> &shader, const std::shared_ptr<Camera> &camera) const;

        unsigned int cubemapTexture = 0;
        Cube cube;
    };

}// namespace voxie
