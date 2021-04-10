//
// Created by Stefan Annell on 2021-03-21.
//

#ifndef GRAPHICSLIGHTNING_CUBE_H
#define GRAPHICSLIGHTNING_CUBE_H

#include <glew.h>
#include <glfw3.h>
#include <vector>

#include "Shader.h"

struct Material {
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Position {
    float x;
    float y;
    float z;
};

struct Dimensions {
    float width;
    float height;
    float depth;
};

class Cube {
public:
    Cube() {}
    Cube(Position p, Dimensions d, Material m = {})
    : nVertices(36), position(p), dimensions(d), material(m) {
        GenerateVertices(position, dimensions);
        CreateBuffers(&vertex[0], sizeof(float) * vertex.size());
    }

    void GenerateVertices(Position p, Dimensions d) {
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1);

        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1);

        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0);

        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0);

        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0);

        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0);
    }

    void SetShaderParameters(Shader& shader) {
        shader.setVec3("objectColor", material.color);
        shader.setVec3("material.ambient", material.ambient);
        shader.setVec3("material.diffuse", material.diffuse);
        shader.setVec3("material.specular", material.specular);
        shader.setFloat("material.shininess", material.shininess);
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
    }

    void SetVertexAttrib(GLuint id, GLint size, GLenum type, GLsizei stride, const void* ptr)  {
        glVertexAttribPointer(attributes, id, size, type, stride, ptr);
        glEnableVertexAttribArray(attributes++);
    }

    void SetPosition(const Position& updatedPosition) {
        position = updatedPosition;
    }

    glm::vec3 GetPosition() const {
        return {position.x, position.y, position.z};
    }

    ~Cube() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void Draw() const {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, nVertices);
    }

private:
    void CreateBuffers(float v[], size_t size) {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, v, GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }

    unsigned int VBO;
    unsigned int VAO;
    int nVertices;
    int attributes = 0;
    Position position;
    Dimensions dimensions;
    Material material;
    std::vector<float> vertex;
};

#endif //GRAPHICSLIGHTNING_CUBE_H
