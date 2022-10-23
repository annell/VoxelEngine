//
// Created by Stefan Annell on 2021-04-10.
//

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "Node.h"
#include "RenderingHandler.h"

namespace voxie {
    class Cube;
    class Shader;
    class VertexBufferArray;
    class Position;
    class Name;
    class ChunkPos;

    class WorldChunk : public NodeWrapper {
    public:
        WorldChunk(const Handle &, std::shared_ptr<Shader>, const ChunkPos &);

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        COMPONENT_ADD_FUNCTION(Position);
        COMPONENT_ADD_FUNCTION(Shader);
        COMPONENT_ADD_FUNCTION(VertexBufferArray);
        COMPONENT_ADD_FUNCTION(Name);

        RenderingConfig GetRenderingConfig() const override;

    private:
        struct ChunkCoordinate {
            bool operator<(const ChunkCoordinate &pos) const {
                return std::tie(x, y, z) < std::tie(pos.x, pos.y, pos.z);
            }
            int x;
            int y;
            int z;
        };

        void SetupCubesForRendering();
        void SetupShader();
        void FaceCulling();
        auto GetPostDrawAction() const;
        auto GetPreDrawAction(const std::shared_ptr<Shader> &shader, const glm::mat4 &model) const;

        std::vector<float> VerticesToRender;

        int nrMaterials = 0;
        std::vector<Cube *> CubesToRender;
        std::map<ChunkCoordinate, Cube> Cubes;
    };

}// namespace voxie
