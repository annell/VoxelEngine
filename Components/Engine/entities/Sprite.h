//
// Created by Stefan Annell on 2021-08-21.
//

#pragma once

#include "EntityComponentSystem.h"
#include "LoadTexture.h"
#include <Node.h>
#include <RenderingHandler.h>
#include <memory>

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
        RenderingConfig GetRenderingConfig() const override;
        COMPONENT_ADD_FUNCTION(Position2D);
        COMPONENT_ADD_FUNCTION(Shader);

        std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;

    private:
        void Setup() const;

        std::shared_ptr<VertexBufferArray> vertexBufferArray;
        Texture2D texture;
        std::string path;
    };

}// namespace voxie
