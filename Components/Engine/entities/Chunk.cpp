//
// Created by Stefan Annell on 2021-04-10.
//

#include "Chunk.h"
#include "Core.h"
#include "Cube.h"
#include "ModelLoader.h"
#include <map>
#include <utility>

namespace voxie {

    Chunk::Chunk(const Handle &handle, const std::string &path, std::shared_ptr<Name> name, std::shared_ptr<Shader> shader, std::shared_ptr<Position> position)
        : NodeWrapper(handle), path(path) {
        COMPONENT_REGISTER(Position, position);
        COMPONENT_REGISTER(Shader, shader);
        COMPONENT_REGISTER(Name, name);
        COMPONENT_REGISTER(VertexBufferArray, std::move(std::make_shared<VertexBufferArray>()));
        COMPONENT_REGISTER(Outline, std::make_shared<Outline>());
        voxie::ModelLoader::LoadModel(path, this);
        SetupCubesForRendering();
        SetupShader();

        auto outline = GetOutline();

        outline->shader = std::make_shared<voxie::Shader>(
                std::map<std::string, ShaderType>{
                        std::make_pair(BASE_PATH + SHADERS + "/outline.vs", ShaderType::VERTEX),
                        std::make_pair(BASE_PATH + SHADERS + "/outline.fs", ShaderType::FRAGMENT)});
        outline->vertexBufferArray = GetVertexBufferArray();
        outline->position = GetPosition();
    }

    void Chunk::encode(YAML::Node &node) const {
        node["type"] = "Chunk";
        node["id"] = GetHandle().GetId();
        node["path"] = path;
        auto name = helper::GetComponent<Name>(handle).get();
        node["name"] = name->name;
        node["position"] = *helper::GetComponent<Position>(handle).get();
    }

    bool Chunk::decode(const YAML::Node &node) {
        GetPosition()->decode(node["position"]);

        auto outline = GetOutline();

        outline->shader = std::make_shared<voxie::Shader>(
                std::map<std::string, ShaderType>{
                        std::make_pair(BASE_PATH + SHADERS + "/outline.vs", ShaderType::VERTEX),
                        std::make_pair(BASE_PATH + SHADERS + "/outline.fs", ShaderType::FRAGMENT)});
        outline->vertexBufferArray = GetVertexBufferArray();
        outline->position = GetPosition();

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
                for (const auto &vertex : cube->GetVertices()) {
                    verticesToRender.push_back(vertex);
                }
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
        vba->SetVertexAttrib(3, 7 * sizeof(float), (void *) (0 * sizeof(float)));
        vba->SetVertexAttrib(3, 7 * sizeof(float), (void *) (3 * sizeof(float)));
        vba->SetVertexAttrib(1, 7 * sizeof(float), (void *) (6 * sizeof(float)));
        shader->setMat4("model", GetPosition()->model);
    }

    void Chunk::AddCube(ChunkPosition position, std::unique_ptr<Cube> &&cube) {
        cubesMap[position] = std::move(cube);
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

    auto Chunk::GetPreDrawAction(const std::shared_ptr<Shader> &shader, const glm::mat4 &model) const {
        return [=]() {
            shader->use();
            shader->setMat4("model", model);
        };
    }

    auto Chunk::GetPostDrawAction() const {
        return [&]() {
            GetOutline()->Render();
        };
    }

    RenderingConfig Chunk::GetRenderingConfig() const {
        return {
                GetShader(),
                GetVertexBufferArray(),
                GetPreDrawAction(GetShader(), GetPosition()->model),
                GetPostDrawAction(),
                [&]() {
                    RenderingInterface::Draw(GetVertexBufferArray());
                }};
    }

}// namespace voxie
