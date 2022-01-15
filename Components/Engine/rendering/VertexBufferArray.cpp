#include "VertexBufferArray.h"

#include <GL/glew.h>
#include <yaml-cpp/yaml.h>

namespace voxie {


    VertexBufferArray::~VertexBufferArray() {
        ResetBuffers();
    }

    void VertexBufferArray::encode(YAML::Node &node) const {
        //node["vertexAttributes"] = vertexAttributes;
        node["nrVertex"] = nrVertex;
        node["attributes"] = attributes;
    }

    bool VertexBufferArray::decode(const YAML::Node &node) {
        return true;
    }

    void VertexBufferArray::CreateBuffers() {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexAttributes.size(), &vertexAttributes[0], GL_STATIC_DRAW);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }

    void VertexBufferArray::ResetBuffers() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void VertexBufferArray::SetVertexAttrib(unsigned int size, unsigned int type, bool normalized, int stride, const void *ptr) {
        glVertexAttribPointer(attributes, size, type, normalized, stride, ptr);
        glEnableVertexAttribArray(attributes++);
    }
}// namespace voxie