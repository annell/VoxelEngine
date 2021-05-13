//
// Created by Stefan Annell on 2021-04-10.
//

#include "Chunk.h"
#include <map>
#include <glm.hpp>

namespace engine::entities {

Chunk::Chunk(rendering::Shader shader, Position position)
 : shader(shader), position(position) {

}

Chunk::~Chunk() {
    ResetBuffers();
}

void Chunk::Init() {
    SetupCubesForRendering();
    SetupShader();
}

void SetVertexAttrib(GLuint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr, int& attributes)  {
    glVertexAttribPointer(attributes, size, type, normalized, stride, ptr);
    glEnableVertexAttribArray(attributes++);
}

void Chunk::SetupCubesForRendering() {
    FaceCulling();
    for (auto& pair : cubesMap) {
        auto& cube = pair.second;
        cube->GenerateVertexAttributes();
        if (cube->GetMaterialIndex() > nrMaterials) {
            nrMaterials = cube->GetMaterialIndex();
        }
        if (cube->ShouldRender()) {
            cubesToRender.push_back(cube.get());
        }
    }
}

void Chunk::SetupShader() {
    shader.use();
    for (auto cube : cubesToRender) {
        nrVertex += cube->GetNrVertex();
        auto& material = cube->GetMaterial();
        std::string index = std::to_string(cube->GetMaterialIndex());
        shader.setVec3("materials["+ index +"].ambient", material.ambient);
        shader.setVec3("materials["+ index +"].diffuse", material.diffuse);
        shader.setVec3("materials["+ index +"].specular", material.specular);
        shader.setFloat("materials["+ index +"].shininess", material.shininess);
        const auto& cubesVertexAttributes = cube->GetVertexAttributes();
        vertexAttributes.insert(vertexAttributes.end(), cubesVertexAttributes.begin(), cubesVertexAttributes.end());
    }

    CreateBuffers(&vertexAttributes[0], sizeof(float) * vertexAttributes.size());
    int attributes = 0;
    SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0, attributes);
    SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)), attributes);
    SetVertexAttrib(1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)), attributes);
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
}

void Chunk::Draw() const {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, NrVertex());
    glDisable(GL_CULL_FACE);
}

void Chunk::AddCube(std::unique_ptr<Cube>&& cube) {
    cubesMap[cube->GetChunkPosition()] = std::move(cube);
}

size_t Chunk::NrMaterials() const {
    return nrMaterials;
}

size_t Chunk::NrVertex() const {
    return nrVertex;
}

void Chunk::FaceCulling() const {
    auto render = [&] (Cube* cube, Cube::Face face, const ChunkPosition& pos) {
        auto it = cubesMap.find(pos);
        if (it != cubesMap.end()) {
            cube->SetRenderSide(face, false);
        }
    };
    for (auto& pair : cubesMap) {
        auto cube = pair.second.get();
        auto dimensions = cube->GetDimensions();
        auto pos = cube->GetChunkPosition();
        pos.x++;
        render(cube, Cube::Face::RIGHT, pos);
        pos.x--;
        pos.x--;
        render(cube, Cube::Face::LEFT, pos);
        pos.x++;

        pos.y++;
        render(cube, Cube::Face::TOP, pos);
        pos.y--;
        pos.y--;
        render(cube, Cube::Face::BOTTOM, pos);
        pos.y++;

        pos.z++;
        render(cube, Cube::Face::FRONT, pos);
        pos.z--;
        pos.z--;
        render(cube, Cube::Face::BACK, pos);
        pos.z++;
    }
}

void Chunk::CreateBuffers(float v[], size_t size) {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, v, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void Chunk::ResetBuffers() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

const Position& Chunk::GetPosition() const {
    return position;
}

void Chunk::SetPosition(Position pos) {
    position = pos;
    glm::vec3 vecPos = {position.x, position.y, position.z};
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, vecPos);
    shader.use();
    shader.setMat4("model", model);
}

rendering::Shader* Chunk::GetShader() {
    return &shader;
}

}
