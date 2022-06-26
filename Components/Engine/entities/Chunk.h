//
// Created by Stefan Annell on 2021-04-10.
//

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "EntityComponentSystem.h"
#include "Node.h"
#include "RenderingHandler.h"
#include "RigidBody.h"

namespace voxie {
    class Cube;
    class Shader;
    class Name;
    class VertexBufferArray;
    class Outline;

    struct ChunkPosition {
        int x;
        int y;
        int z;

        bool operator<(const ChunkPosition &pos) const {
            return std::tie(x, y, z) < std::tie(pos.x, pos.y, pos.z);
        }
    };

    class Chunk : public NodeWrapper {
    public:
        Chunk(const Handle &, const std::string &path, std::shared_ptr<Name>, std::shared_ptr<Shader>, std::shared_ptr<Position>);

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        void AddCube(ChunkPosition, std::unique_ptr<Cube> &&);

        COMPONENT_ADD_FUNCTION(Position);
        COMPONENT_ADD_FUNCTION(Name);
        COMPONENT_ADD_FUNCTION(Shader);
        COMPONENT_ADD_FUNCTION(VertexBufferArray);
        COMPONENT_ADD_FUNCTION(Outline);

        RenderingConfig GetRenderingConfig() const override;

    private:
        void UpdateChunkMaxMin(const ChunkPosition &);
        void SetupCubesForRendering();
        void SetupShader();
        void FaceCulling() const;
        auto GetPostDrawAction() const;
        auto GetPreDrawAction(const std::shared_ptr<Shader> &shader, const glm::mat4 &model) const;

        size_t nrMaterials = 0;
        std::vector<Cube *> cubesToRender;
        std::string path;
        std::vector<float> verticesToRender;

        std::map<ChunkPosition, std::unique_ptr<Cube>> cubesMap;
        ChunkAxises chunkMaxMins;
    };

}// namespace voxie
