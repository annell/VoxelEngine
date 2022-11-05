//
// Created by Stefan Annell on 2021-04-10.
//

#pragma once

#include <array>
#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Cube.h"
#include "Node.h"
#include "RenderingHandler.h"

namespace voxie {

    struct ChunkCoordinate {
        int x;
        int y;
        int z;
        bool operator==(const voxie::ChunkCoordinate &rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }
    };
}// namespace voxie

template<>
struct std::hash<voxie::ChunkCoordinate> {
    std::size_t operator()(const voxie::ChunkCoordinate &s) const noexcept {
        std::size_t h1 = std::hash<int>{}(s.x);
        std::size_t h2 = std::hash<int>{}(s.y);
        std::size_t h3 = std::hash<int>{}(s.x);
        return ((h1 ^ (h2 << 1)) >> 1 ^ (h3 << 1));
    }
};

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
        void SetupShader();

    private:
        void SetupCubesForRendering();
        void FaceCulling();
        auto GetPostDrawAction() const;
        auto GetPreDrawAction(const std::shared_ptr<Shader> &shader, const glm::mat4 &model) const;

        std::vector<float> VerticesToRender;

        int nrMaterials = 0;
        std::vector<Cube *> CubesToRender;
        std::array<std::array<std::array<Cube, ChunkPos::zSize>, ChunkPos::ySize>, ChunkPos::xSize> Cubes;
    };

}// namespace voxie