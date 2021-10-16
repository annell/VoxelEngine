//
// Created by Stefan Annell on 2021-04-10.
//

#pragma once

#include "Core.h"
#include <RenderingHandler.h>
#include <map>
#include <memory>
#include <vector>

#include "Cube.h"
#include "EntityComponentSystem.h"
#include "Shader.h"

namespace voxie {

    class Chunk {
    public:
        Chunk(const Entity&, const std::string &path, std::shared_ptr<Name>, std::shared_ptr<Shader>, std::shared_ptr<Position>);
        ~Chunk();

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        void Draw() const;
        void AddCube(ChunkPosition, std::unique_ptr<Cube> &&);
        [[nodiscard]] size_t NrVertex() const;

        [[nodiscard]] std::shared_ptr<Position> GetPosition() const;
        [[nodiscard]] RenderingConfig GetRenderingConfig() const;
        [[nodiscard]] std::shared_ptr<Shader> GetShader() const;
        [[nodiscard]] std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;
        [[nodiscard]] const Entity &GetEntity() const;

    private:
        void SetupCubesForRendering();
        void SetupShader();
        void FaceCulling() const;

        size_t nrMaterials = 0;
        std::vector<Cube *> cubesToRender;
        std::shared_ptr<VertexBufferArray> vertexBufferArray;
        std::string path;

        std::map<ChunkPosition, std::unique_ptr<Cube>> cubesMap;
        Entity entity;
    };

}// namespace voxie
