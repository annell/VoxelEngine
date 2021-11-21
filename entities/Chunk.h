//
// Created by Stefan Annell on 2021-04-10.
//

#pragma once

#include "Core.h"
#include <Node.h>
#include <RenderingHandler.h>
#include <map>
#include <memory>
#include <vector>

#include "Cube.h"
#include "EntityComponentSystem.h"
#include "Shader.h"

namespace voxie {

    class Chunk : public NodeWrapper {
    public:
        Chunk(const Handle &, const std::string &path, std::shared_ptr<Name>, std::shared_ptr<Shader>, std::shared_ptr<Position>);
        ~Chunk();

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        void Draw() const;
        void AddCube(ChunkPosition, std::unique_ptr<Cube> &&);
        [[nodiscard]] size_t NrVertex() const;

        [[nodiscard]] std::shared_ptr<Position> GetPosition() const override;
        [[nodiscard]] RenderingConfig GetRenderingConfig() const;
        [[nodiscard]] std::shared_ptr<Shader> GetShader() const;
        [[nodiscard]] std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;
        [[nodiscard]] const Handle &GetHandle() const override;

    private:
        void SetupCubesForRendering();
        void SetupShader();
        void FaceCulling() const;
        auto GetPostDrawAction() const;
        auto GetPreDrawAction(const std::shared_ptr<Shader> &shader, const glm::mat4 &model) const;

        size_t nrMaterials = 0;
        std::vector<Cube *> cubesToRender;
        std::shared_ptr<VertexBufferArray> vertexBufferArray;
        std::string path;

        std::map<ChunkPosition, std::unique_ptr<Cube>> cubesMap;
        Handle handle;
    };

}// namespace voxie
