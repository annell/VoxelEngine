#include "VertexBufferArray.h"
#include <yaml-cpp/yaml.h>

namespace voxie {


    VertexBufferArray::~VertexBufferArray() {
        ResetBuffers();
    }

    void VertexBufferArray::encode(YAML::Node &node) const {
    }

    bool VertexBufferArray::decode(const YAML::Node &node) {
        return true;
    }

    void VertexBufferArray::CreateBuffers() {
        buffers = RenderingInterface::CreateBuffers(vertexAttributes);
    }

    void VertexBufferArray::ResetBuffers() {
        RenderingInterface::ResetBuffers(buffers);
    }

    void VertexBufferArray::SetVertexAttrib(unsigned int size, int stride, const void *ptr) {
        RenderingInterface::SetVertexAttrib(attributes++, size, stride, ptr);
    }
}// namespace voxie