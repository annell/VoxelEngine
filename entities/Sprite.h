//
// Created by Stefan Annell on 2021-08-21.
//

#pragma once

#include "BaseComponents.h"
#include "Core.h"
#include "EntityComponentSystem.h"
#include <RenderingHandler.h>
#include <Shader.h>
#include <memory>

namespace voxie {
    class Sprite {
    public:
        using Texture2D = unsigned int;
        Sprite(std::string path, std::shared_ptr<Name>, std::shared_ptr<Shader>, std::shared_ptr<Position2D>);

        void Draw() const;
        RenderingConfig GetRenderingConfig() const;
        std::shared_ptr<Position2D> GetPosition() const;
        std::shared_ptr<Shader> GetShader() const;
        std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;

        const Entity &GetEntity() const;

    private:
        void Setup() const;

        Entity entity;
        Texture2D texture;
    };

}// namespace voxie