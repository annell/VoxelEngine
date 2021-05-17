//
// Created by Stefan Annell on 2021-04-10.
//

#include "Chunk.h"
#include <map>
#include "Engine.h"

namespace voxie {

Chunk::Chunk(std::string name, std::shared_ptr<Shader> shader, std::shared_ptr<Position> position)
 : entity(Entity::MakeEntity(name)) {
    helper::AddComponent(position, *entity);
    helper::AddComponent(shader, *entity);
    helper::AddComponent(std::make_shared<VertexBufferArray>(), *entity);
}

Chunk::~Chunk() {
    GetVertexBufferArray()->ResetBuffers();
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
    auto shader = GetShader();
    auto vertexBufferArray = GetVertexBufferArray();
    auto position = GetPosition();
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
    vertexBufferArray->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    vertexBufferArray->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    vertexBufferArray->SetVertexAttrib(1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    shader->setMat4("model", GetPosition()->model);
}

void Chunk::Draw() const {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    GetShader()->use();
    glBindVertexArray(GetVertexBufferArray()->VAO);
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
    return GetVertexBufferArray()->nrVertex;
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

std::shared_ptr<Position> Chunk::GetPosition() const {
    return voxie::helper::GetComponent<Position>(*entity);
}

void Chunk::SetPosition(Position pos) {
    GetPosition()->SetPosition(pos.x, pos.y, pos.z);
    GetShader()->use();
    GetShader()->setMat4("model", GetPosition()->model);
}

std::shared_ptr<Shader> Chunk::GetShader() const {
    return voxie::helper::GetComponent<Shader>(*entity);
}

std::shared_ptr<VertexBufferArray> Chunk::GetVertexBufferArray() const {
    return voxie::helper::GetComponent<VertexBufferArray>(*entity);
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

RenderingConfig Chunk::GetRenderingConfig() const {
    return {
        GetShader(),
        GetVertexBufferArray(),
        GetPreDrawAction(),
        GetPostDrawAction()
    };
}

std::shared_ptr<Entity> Chunk::GetEntity() const {
    return entity;
}

}
