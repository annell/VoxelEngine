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
    Cube(Position p, Dimensions d, Material m = {}, int materialIndex = -1, int i=0)
    : nVertices(42), position(p), dimensions(d), material(m), materialIndex(materialIndex), i(i) {
        GenerateVertices(position, dimensions);
        CreateBuffers(&vertex[0], sizeof(float) * vertex.size());
    }

    ~Cube() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void GenerateVertices(Position p, Dimensions d) {
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(-1); vertex.push_back((float)materialIndex);

        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(0); vertex.push_back(1); vertex.push_back((float)materialIndex);

        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(-1) ;vertex.push_back(0) ;vertex.push_back(0); vertex.push_back((float)materialIndex);

        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(1); vertex.push_back(0); vertex.push_back(0); vertex.push_back((float)materialIndex);

        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y - (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(-1) ;vertex.push_back(0); vertex.push_back((float)materialIndex);

        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x + (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z + (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0); vertex.push_back((float)materialIndex);
        vertex.push_back(p.x - (d.width) / 2); vertex.push_back(p.y + (d.height) / 2); vertex.push_back(p.z - (d.depth) / 2); vertex.push_back(0); vertex.push_back(1); vertex.push_back(0); vertex.push_back((float)materialIndex);
    }

    void SetVertexAttrib(GLuint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)  {
        glVertexAttribPointer(attributes, size, type, normalized, stride, ptr);
        glEnableVertexAttribArray(attributes++);
    }

    void SetIntVertexAttrib(GLuint size, GLenum type, GLsizei stride, const void* ptr)  {
        glVertexAttribIPointer(attributes, size, type, stride, ptr);
        glEnableVertexAttribArray(attributes++);
    }

    void SetPosition(const Position& updatedPosition) {
        position = updatedPosition;
    }

    glm::vec3 GetPosition() const {
        return {position.x, position.y, position.z};
    }

    const Material& GetMaterial() const {
        return material;
    }

    const int& GetMaterialIndex() const {
        return materialIndex;
    }

    size_t GetNrVertex() const {
        return nVertices;
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
    int i = 0;
    Position position;
    Dimensions dimensions;
    Material material;
    int materialIndex;
    std::vector<float> vertex;
};

#endif //GRAPHICSLIGHTNING_CUBE_H
