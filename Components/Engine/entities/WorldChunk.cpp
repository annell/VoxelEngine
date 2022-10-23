//
// Created by Stefan Annell on 2021-04-10.
//

#include "WorldChunk.h"
#include "ChunkPos.h"
#include "Core.h"
#include "Cube.h"
#include "PerlinNoise.h"
#include <map>
#include <random>
#include <utility>

namespace voxie {

    WorldChunk::WorldChunk(const Handle &handle, std::shared_ptr<Shader> shader, const ChunkPos &chunkPos)
        : NodeWrapper(handle) {
        auto pos = std::make_shared<Position>(chunkPos.toPos());
        COMPONENT_REGISTER(Position, pos);
        COMPONENT_REGISTER(Shader, shader);
        std::string name = "WorldChunkAt - " + std::to_string(chunkPos.x) + ", " + std::to_string(chunkPos.y);
        COMPONENT_REGISTER(Name, std::make_shared<Name>(name));
        COMPONENT_REGISTER(VertexBufferArray, std::move(std::make_shared<VertexBufferArray>()));

        const siv::PerlinNoise::seed_type seed = 123456u;
        const siv::PerlinNoise perlin{seed};
        constexpr float frequency = 0.01;

        int index = 0;

        std::vector<Material> materials;
        for (int i = 0; i < 10; i++) {
            float m = (float) i / 10.0f;
            auto material = Material{{0.5, m, 0.5},
                                     {0.5 + 0.2f, m + 0.2f, 0.5 + 0.2f},
                                     {0.5 + 0.1f, m + 0.1f, 0.5 + 0.1f},
                                     32.0f};
            materials.push_back(material);
        }

        for (int x = 0; x < ChunkPos::xSize; x++) {
            for (int z = 0; z < ChunkPos::zSize; z++) {
                auto val = perlin.normalizedOctave2D_01((chunkPos.x * ChunkPos::xSize + x) * frequency, (chunkPos.y * ChunkPos::zSize + z) * frequency, 4);
                for (int y = 0; y < ChunkPos::ySize; y++) {
                    float height = (float) y / (float) ChunkPos::ySize;
                    if (val > height) {
                        int index = (height * 10);
                        Cubes.emplace(ChunkCoordinate{x, y, z}, Cube(Position({(float) x, (float) y, (float) z}), {1, 1, 1}, materials[index], index));
                    }
                }
            }
        }

        SetupCubesForRendering();
        SetupShader();
    }

    void WorldChunk::encode(YAML::Node &node) const {
        node["type"] = "WorldChunks";
        node["id"] = GetHandle().GetId();
        node["position"] = *helper::GetComponent<Position>(handle).get();
    }

    bool WorldChunk::decode(const YAML::Node &node) {
        DecodeComponent(node["position"], GetPosition());

        return true;
    }

    void WorldChunk::SetupCubesForRendering() {
        FaceCulling();
        for (auto &pair : Cubes) {
            auto &cube = pair.second;
            cube.GenerateVertexAttributes();
            if (cube.GetMaterialIndex() > nrMaterials) {
                nrMaterials = cube.GetMaterialIndex();
            }
            if (cube.ShouldRender()) {
                for (const auto &vertex : cube.GetVertices()) {
                    VerticesToRender.push_back(vertex);
                }
                CubesToRender.push_back(&cube);
            }
        }
    }

    void WorldChunk::SetupShader() {
        auto shader = GetShader();
        auto vba = GetVertexBufferArray();
        shader->use();
        for (const auto &cube : CubesToRender) {
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

    void WorldChunk::FaceCulling() {
        auto render = [&](Cube &cube, Cube::Face face, const ChunkCoordinate &pos) {
            auto it = Cubes.find(pos);
            if (it != Cubes.end()) {
                cube.SetRenderSide(face, false);
            }
        };
        for (auto &pair : Cubes) {
            auto &cube = pair.second;
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

    auto WorldChunk::GetPreDrawAction(const std::shared_ptr<Shader> &shader, const glm::mat4 &model) const {
        return [=]() {
            shader->use();
            shader->setMat4("model", model);
        };
    }

    auto WorldChunk::GetPostDrawAction() const {
        return [&]() {
        };
    }

    RenderingConfig WorldChunk::GetRenderingConfig() const {
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
