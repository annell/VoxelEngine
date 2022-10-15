#include "VertexBufferArray.h"
#include <yaml-cpp/yaml.h>

namespace voxie {


    VertexBufferArray::~VertexBufferArray() {
        ResetBuffers();
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