//
// Created by Stefan Annell on 2021-04-10.
//

#include "Chunk.h"
#include "Engine.h"
#include "ModelLoader.h"
#include <map>
#include <utility>

namespace voxie {

    Chunk::Chunk(const Handle & entity, const std::string &path, std::shared_ptr<Name> name, std::shared_ptr<Shader> shader, std::shared_ptr<Position> position)
        : entity(entity), vertexBufferArray(std::move(std::make_shared<VertexBufferArray>())), path(path) {
        helper::AddComponent(entity, std::move(name));
        helper::AddComponent(entity, std::move(position));
        helper::AddComponent(entity, std::move(shader));
        voxie::ModelLoader::LoadModel(path, this);
        SetupCubesForRendering();
        SetupShader();
    }

    Chunk::~Chunk() {
        helper::RemoveComponent<Name>(entity);
        helper::RemoveComponent<Shader>(entity);
        helper::RemoveComponent<Position>(entity);
    }

    void Chunk::encode(YAML::Node &node) const {
        node["type"] = "Chunk";
        node["id"] = GetEntity().GetId();
        node["path"] = path;
        auto name = helper::GetComponent<Name>(entity).get();
        node["name"] = name->name;
        node["position"] = *helper::GetComponent<Position>(entity).get();
    }

    bool Chunk::decode(const YAML::Node &node) {
        GetPosition()->decode(node["position"]);
        return true;
    }

    void Chunk::SetupCubesForRendering() {
        FaceCulling();
        for (auto &pair : cubesMap) {
            auto &cube = pair.second;
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
        auto vba = GetVertexBufferArray();
        shader->use();
        for (auto cube : cubesToRender) {
            vba->nrVertex += cube->GetNrVertex();
            auto &material = cube->GetMaterial();
            std::string index = std::to_string(cube->GetMaterialIndex());
            shader->setVec3("materials[" + index + "].ambient", material.ambient);
            shader->setVec3("materials[" + index + "].diffuse", material.diffuse);
            shader->setVec3("materials[" + index + "].specular", material.specular);
            shader->setFloat("materials[" + index + "].shininess", material.shininess);
            const auto &cubesVertexAttributes = cube->GetVertexAttributes();
            vba->vertexAttributes.insert(vba->vertexAttributes.end(), cubesVertexAttributes.begin(), cubesVertexAttributes.end());
        }

        vba->CreateBuffers();
        vba->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) nullptr);
        vba->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));
        vba->SetVertexAttrib(1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (6 * sizeof(float)));
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

    void Chunk::AddCube(ChunkPosition position, std::unique_ptr<Cube> &&cube) {
        cubesMap[position] = std::move(cube);
    }

    size_t Chunk::NrVertex() const {
        return GetVertexBufferArray()->nrVertex;
    }

    void Chunk::FaceCulling() const {
        auto render = [&](Cube *cube, Cube::Face face, const ChunkPosition &pos) {
            auto it = cubesMap.find(pos);
            if (it != cubesMap.end()) {
                cube->SetRenderSide(face, false);
            }
        };
        for (auto &pair : cubesMap) {
            auto cube = pair.second.get();
            auto pos = pair.first;
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
        return voxie::helper::GetComponent<Position>(entity);
    }

    std::shared_ptr<Shader> Chunk::GetShader() const {
        return voxie::helper::GetComponent<Shader>(entity);
    }

    std::shared_ptr<VertexBufferArray> Chunk::GetVertexBufferArray() const {
        return vertexBufferArray;
    }

    auto GetPreDrawAction(const std::shared_ptr<Shader> &shader, const glm::mat4 &model) {
        return [=]() {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            shader->use();
            shader->setMat4("model", model);
        };
    }

    auto GetPostDrawAction() {
        return []() {
            glDisable(GL_CULL_FACE);
        };
    }

    RenderingConfig Chunk::GetRenderingConfig() const {
        auto obj = Engine::GetEngine().GetScene().GetRoot()->Find(GetEntity());
        auto p0 = GetPosition();
        auto model = p0->GetModel(obj->GetRelativePosition(), p0->scale, obj->GetRelativeRotation());
        return {
                GetShader(),
                GetVertexBufferArray(),
                GetPreDrawAction(GetShader(), model),
                GetPostDrawAction(),
                [&]() {
                    glBindVertexArray(GetVertexBufferArray()->VAO);
                    glDrawArrays(GL_TRIANGLES, 0, GetVertexBufferArray()->nrVertex);
                }};
    }

    const Handle &Chunk::GetEntity() const {
        return entity;
    }

}// namespace voxie
