//
// Created by Stefan Annell on 2021-05-13.
//

#pragma once
#include <vector>

namespace YAML {
    class Node;
}

namespace voxie {

    struct VertexBufferArray {

        ~VertexBufferArray();
        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);

        void CreateBuffers();
        void ResetBuffers();
        void SetVertexAttrib(unsigned int size, unsigned int type, bool normalized, int stride, const void *ptr);
        unsigned int VBO;
        unsigned int VAO;
        std::vector<float> vertexAttributes;
        size_t nrVertex = 0;
        int attributes = 0;
    };

}// namespace voxie