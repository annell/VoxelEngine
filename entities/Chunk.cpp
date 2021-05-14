//
// Created by Stefan Annell on 2021-04-10.
//

#include "Chunk.h"
#include <map>

namespace engine::entities {

Chunk::Chunk(std::shared_ptr<rendering::Shader> shader, Position position)
 : shader(shader)
 , position(position)
 , vertexBufferArray(std::make_shared<rendering::VertexBufferArray>()) {

}

Chunk::~Chunk() {
    vertexBufferArray->ResetBuffers();
}

void Chunk::Init() {
    SetupCubesForRendering();
    SetupShader();
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
    shader->use();
    for (auto cube : cubesToRender) {
        vertexBufferArray->nrVertex += cube->GetNrVertex();
        auto& material = cube->GetMaterial();
        std::string index = std::to_string(cube->GetMaterialIndex());
        shader->setVec3("materials["+ index +"].ambient", material.ambient);
        shader->setVec3("materials["+ index +"].diffuse", material.diffuse);
        shader->setVec3("materials["+ index +"].specular", material.specular);
        shader->setFloat("materials["+ index +"].shininess", material.shininess);
        const auto& cubesVertexAttributes = cube->GetVertexAttributes();
        vertexBufferArray->vertexAttributes.insert(vertexBufferArray->vertexAttributes.end(), cubesVertexAttributes.begin(), cubesVertexAttributes.end());
    }

    vertexBufferArray->CreateBuffers();
    int attributes = 0;
    vertexBufferArray->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    vertexBufferArray->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    vertexBufferArray->SetVertexAttrib(1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);
}

void Chunk::Draw() const {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    shader->use();
    glBindVertexArray(vertexBufferArray->VAO);
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
    return vertexBufferArray->nrVertex;
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

const Position& Chunk::GetPosition() const {
    return position;
}

void Chunk::SetPosition(Position pos) {
    position = pos;
    glm::vec3 vecPos = {position.x, position.y, position.z};
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, vecPos);
    shader->use();
    shader->setMat4("model", model);
}

std::shared_ptr<rendering::Shader> Chunk::GetShader() const {
    return shader;
}

std::shared_ptr<rendering::VertexBufferArray> Chunk::GetVertexBufferArray() const {
    return vertexBufferArray;
}

auto GetPreDrawAction() {
    return [] () {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    };
}

auto GetPostDrawAction() {
    return [] () {
        glDisable(GL_CULL_FACE);
    };
}

rendering::RenderingConfig Chunk::GetRenderingConfig() const {
    return {
        GetShader(),
        GetVertexBufferArray(),
        GetPreDrawAction(),
        GetPostDrawAction()
    };
}

}
