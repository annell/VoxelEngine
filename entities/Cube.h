//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include <glew.h>
#include <glfw3.h>
#include <vector>

#include "Shader.h"
#include "VertexBufferArray.h"

namespace voxie {

struct Material {
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Position {
    Position(float x, float y, float z)
    : x(x), y(y), z(z), model(glm::mat4(1.0f)) {}

    Position(const Position& p)
    : Position(p.x, p.y, p.z) {}

    bool operator<(const Position& pos) const {
        return std::tie(x, y, z) < std::tie(pos.x, pos.y, pos.z);
    }

    void SetPosition(float x, float y, float z) {
        this->x = x; this->y = y; this->z = z;
        this->model = glm::translate(glm::mat4(1.0f), {x, y, z});
    }

    float x;
    float y;
    float z;
    glm::mat4 model;
};

struct ChunkPosition {
    int x;
    int y;
    int z;

    bool operator<(const ChunkPosition& pos) const {
        return std::tie(x, y, z) < std::tie(pos.x, pos.y, pos.z);
    }
};

struct Dimensions {
    float width;
    float height;
    float depth;
};

class Cube {
public:
struct Triangle {
    float vertex[9];
};
enum Face {
    BACK = 0,
    FRONT = 1,
    LEFT = 2,
    RIGHT = 3,
    BOTTOM = 4,
    TOP = 5,
};
struct Side {
    Triangle triangle1;    
    Triangle triangle2;    
    float normal[3];
    float materialIndex;
    bool render = true;
};
    Cube(Position p, Dimensions d, Material m = {}, int materialIndex = -1)
    : position(p)
    , dimensions(d)
    , material(m)
    , materialIndex(materialIndex)
    , vertexBufferArray(std::make_shared<VertexBufferArray>()) {
        GenerateSides(position, dimensions);
    }

    ~Cube() {
    }

    void GenerateVertexAttributes() {
        for (auto side : sides) {
            if (side.render) {
                GenerateVerexAttributes(side);
                vertexBufferArray->nrVertex += 6;
            }
        }
    }

    void CreateRenderBuffers() {
        if (ShouldRender()) {
            vertexBufferArray->CreateBuffers();
        }
    }

    const std::vector<float>& GetVertexAttributes() {
        return vertexBufferArray->vertexAttributes;
    }

    void SetVertexAttrib(GLuint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)  {
        vertexBufferArray->SetVertexAttrib(size, type, normalized, stride, ptr);
    }

    void SetRenderSide(Face face, bool render) {
        sides[face].render = render;        
    }

    ChunkPosition GetChunkPosition() const {
        return chunkPosition;
    }

    void SetChunkPosition(const ChunkPosition& updatedPosition) {
        chunkPosition = updatedPosition;
    }

    __unused glm::vec3 GetPosition() const {
        return {position.x, position.y, position.z};
    }

    Dimensions GetDimensions() const {
        return dimensions;
    }

    const Material& GetMaterial() const {
        return material;
    }

    const int& GetMaterialIndex() const {
        return materialIndex;
    }

    size_t GetNrVertex() const {
        return vertexBufferArray->nrVertex;
    }

    void Draw() const {
        if (ShouldRender()) {
            glBindVertexArray(vertexBufferArray->VAO);
            glDrawArrays(GL_TRIANGLES, 0, GetNrVertex());
        }
    }

    bool ShouldRender() const {
        return GetNrVertex() != 0;
    }

    std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const {
        return vertexBufferArray;
    }

private:
    void GenerateVerexAttributes(const Side& side) {
        int index = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                vertexBufferArray->vertexAttributes.push_back(side.triangle1.vertex[index]);
                index++;
            }
            for (int ii = 0; ii < 3; ii++) {
                vertexBufferArray->vertexAttributes.push_back(side.normal[ii]);
            }
            vertexBufferArray->vertexAttributes.push_back(side.materialIndex);
        }
        index = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                vertexBufferArray->vertexAttributes.push_back(side.triangle2.vertex[index]);
                index++;
            }
            for (int ii = 0; ii < 3; ii++) {
                vertexBufferArray->vertexAttributes.push_back(side.normal[ii]);
            }
            vertexBufferArray->vertexAttributes.push_back(side.materialIndex);
        }
    }

    void GenerateSides(Position p, Dimensions d) {
        sides[Face::BACK].materialIndex = materialIndex;
        sides[Face::BACK].normal[0] = 0;
        sides[Face::BACK].normal[1] = 0;
        sides[Face::BACK].normal[2] = -1;
        sides[Face::BACK].triangle1 = {
            p.x - (d.width) / 2, p.y - (d.height) / 2, p.z - (d.depth) / 2,
            p.x + (d.width) / 2, p.y - (d.height) / 2, p.z - (d.depth) / 2,
            p.x + (d.width) / 2, p.y + (d.height) / 2, p.z - (d.depth) / 2
        };
        sides[Face::BACK].triangle2 = {
            p.x + (d.width) / 2, p.y + (d.height) / 2, p.z - (d.depth) / 2,
            p.x - (d.width) / 2, p.y + (d.height) / 2, p.z - (d.depth) / 2,
            p.x - (d.width) / 2, p.y - (d.height) / 2, p.z - (d.depth) / 2
        };

        sides[Face::FRONT].materialIndex = materialIndex;
        sides[Face::FRONT].normal[0] = 0;
        sides[Face::FRONT].normal[1] = 0;
        sides[Face::FRONT].normal[2] = 1;
        sides[Face::FRONT].triangle1 = {
            p.x - (d.width) / 2, p.y - (d.height) / 2, p.z + (d.depth) / 2,
            p.x + (d.width) / 2, p.y + (d.height) / 2, p.z + (d.depth) / 2,
            p.x + (d.width) / 2, p.y - (d.height) / 2, p.z + (d.depth) / 2
        };
        sides[Face::FRONT].triangle2 = {
            p.x + (d.width) / 2, p.y + (d.height) / 2, p.z + (d.depth) / 2,
            p.x - (d.width) / 2, p.y - (d.height) / 2, p.z + (d.depth) / 2,
            p.x - (d.width) / 2, p.y + (d.height) / 2, p.z + (d.depth) / 2
        };

        sides[Face::LEFT].materialIndex = materialIndex;
        sides[Face::LEFT].normal[0] = -1;
        sides[Face::LEFT].normal[1] = 0;
        sides[Face::LEFT].normal[2] = 0;
        sides[Face::LEFT].triangle1 = {
            p.x - (d.width) / 2, p.y + (d.height) / 2, p.z + (d.depth) / 2,
            p.x - (d.width) / 2, p.y - (d.height) / 2, p.z - (d.depth) / 2,
            p.x - (d.width) / 2, p.y + (d.height) / 2, p.z - (d.depth) / 2
        };
        sides[Face::LEFT].triangle2 = {
            p.x - (d.width) / 2, p.y - (d.height) / 2, p.z - (d.depth) / 2,
            p.x - (d.width) / 2, p.y + (d.height) / 2, p.z + (d.depth) / 2,
            p.x - (d.width) / 2, p.y - (d.height) / 2, p.z + (d.depth) / 2
        };

        sides[Face::RIGHT].materialIndex = materialIndex;
        sides[Face::RIGHT].normal[0] = 1;
        sides[Face::RIGHT].normal[1] = 0;
        sides[Face::RIGHT].normal[2] = 0;
        sides[Face::RIGHT].triangle1 = {
            p.x + (d.width) / 2, p.y + (d.height) / 2, p.z + (d.depth) / 2,
            p.x + (d.width) / 2, p.y + (d.height) / 2, p.z - (d.depth) / 2,
            p.x + (d.width) / 2, p.y - (d.height) / 2, p.z - (d.depth) / 2
        };
        sides[Face::RIGHT].triangle2 = {
            p.x + (d.width) / 2, p.y - (d.height) / 2, p.z - (d.depth) / 2,
            p.x + (d.width) / 2, p.y - (d.height) / 2, p.z + (d.depth) / 2,
            p.x + (d.width) / 2, p.y + (d.height) / 2, p.z + (d.depth) / 2
        };

        sides[Face::BOTTOM].materialIndex = materialIndex;
        sides[Face::BOTTOM].normal[0] = 0;
        sides[Face::BOTTOM].normal[1] = -1;
        sides[Face::BOTTOM].normal[2] = 0;
        sides[Face::BOTTOM].triangle1 = {
            p.x - (d.width) / 2, p.y - (d.height) / 2, p.z - (d.depth) / 2,
            p.x + (d.width) / 2, p.y - (d.height) / 2, p.z + (d.depth) / 2,
            p.x + (d.width) / 2, p.y - (d.height) / 2, p.z - (d.depth) / 2
        };
        sides[Face::BOTTOM].triangle2 = {
            p.x + (d.width) / 2, p.y - (d.height) / 2, p.z + (d.depth) / 2,
            p.x - (d.width) / 2, p.y - (d.height) / 2, p.z - (d.depth) / 2,
            p.x - (d.width) / 2, p.y - (d.height) / 2, p.z + (d.depth) / 2
        };

        sides[Face::TOP].materialIndex = materialIndex;
        sides[Face::TOP].normal[0] = 0;
        sides[Face::TOP].normal[1] = 1;
        sides[Face::TOP].normal[2] = 0;
        sides[Face::TOP].triangle1 = {
            p.x - (d.width) / 2, p.y + (d.height) / 2, p.z - (d.depth) / 2,
            p.x + (d.width) / 2, p.y + (d.height) / 2, p.z - (d.depth) / 2,
            p.x + (d.width) / 2, p.y + (d.height) / 2, p.z + (d.depth) / 2
        };
        sides[Face::TOP].triangle2 = {
            p.x + (d.width) / 2, p.y + (d.height) / 2, p.z + (d.depth) / 2,
            p.x - (d.width) / 2, p.y + (d.height) / 2, p.z + (d.depth) / 2,
            p.x - (d.width) / 2, p.y + (d.height) / 2, p.z - (d.depth) / 2
        };
    }

    std::shared_ptr<VertexBufferArray> vertexBufferArray;

    Position position;
    ChunkPosition chunkPosition;
    Dimensions dimensions;
    Material material;
    int materialIndex;
    Side sides[6];
};

}
