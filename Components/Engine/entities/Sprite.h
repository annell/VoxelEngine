//
// Created by Stefan Annell on 2021-08-21.
//

#pragma once

#include "EntityComponentSystem.h"
#include <Node.h>
#include <RenderingHandler.h>
#include <memory>
#include "LoadTexture.h"

namespace voxie {
    class Position2D;
    class Shader;
    class Name;
    class VertexBufferArray;

    class Sprite : public NodeWrapper {
    public:
        Sprite(const Handle &, std::string path, std::shared_ptr<Name>, std::shared_ptr<Shader>, std::shared_ptr<Position2D>);
        ~Sprite();

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        void Draw() const;
        RenderingConfig GetRenderingConfig() const;
        std::shared_ptr<Position2D> GetPosition2D() const;
        std::shared_ptr<Shader> GetShader() const;
        std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;

        const Handle &GetHandle() const override;

    private:
        void Setup() const;

        std::shared_ptr<VertexBufferArray> vertexBufferArray;
        Handle handle;
        Texture2D texture;
        std::string path;
    };

}// namespace voxie
