//
// Created by Stefan Annell on 2022-04-03.
//

#pragma once
#include "memory"

namespace voxie {

    struct Shader;
    struct VertexBufferArray;
    struct Position;

    struct InvertedHull {
        float scale = 1.02;

        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexBufferArray> vertexBufferArray;
        std::shared_ptr<Position> position;

        void Render() const;
    };
}// namespace voxie
