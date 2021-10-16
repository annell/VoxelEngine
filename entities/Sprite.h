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
        Sprite(const Entity&, std::string path, std::shared_ptr<Name>, std::shared_ptr<Shader>, std::shared_ptr<Position2D>);
        ~Sprite();

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        void Draw() const;
        [[nodiscard]] RenderingConfig GetRenderingConfig() const;
        [[nodiscard]] std::shared_ptr<Position2D> GetPosition() const;
        [[nodiscard]] std::shared_ptr<Shader> GetShader() const;
        [[nodiscard]] std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;

        [[nodiscard]] const Entity &GetEntity() const;

    private:
        void Setup() const;

        std::shared_ptr<VertexBufferArray> vertexBufferArray;
        Entity entity;
        Texture2D texture;
        std::string path;
    };

}// namespace voxie
