//
// Created by Stefan Annell on 2021-05-13.
//

#pragma once
#include "JsonUtil.h"
#include "RenderingInterface.h"
#include <vector>

namespace YAML {
    class Node;
}

namespace voxie {

    struct VertexBufferArray {

        ~VertexBufferArray();

        void CreateBuffers();
        void ResetBuffers();
        void SetVertexAttrib(unsigned int size, int stride, const void *ptr);
        RenderingInterface::VertexBuffers buffers;
        std::vector<float> vertexAttributes;
        size_t nrVertex = 0;
        int attributes = 0;
    };

}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::VertexBufferArray> {
        static Node encode(const voxie::VertexBufferArray &rhs) {
            Node node;
            return node;
        }

        static bool decode(const Node &node, voxie::VertexBufferArray &rhs) {
            return true;
        }
    };
}// namespace YAML