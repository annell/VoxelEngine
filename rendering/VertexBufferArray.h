//
// Created by Stefan Annell on 2021-05-13.
//

#pragma once
#include <vector>

namespace engine::rendering {

struct VertexBufferArray {
    void CreateBuffers() {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexAttributes.size(), &vertexAttributes[0], GL_STATIC_DRAW);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }

    void ResetBuffers() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void SetVertexAttrib(GLuint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)  {
        glVertexAttribPointer(attributes, size, type, normalized, stride, ptr);
        glEnableVertexAttribArray(attributes++);
    }

    unsigned int VBO;
    unsigned int VAO;
    std::vector<float> vertexAttributes;
    size_t nrVertex = 0;
    int attributes = 0;
};

}