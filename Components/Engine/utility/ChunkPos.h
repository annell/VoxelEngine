#pragma once

#include <glm/glm.hpp>

namespace voxie {
    struct ChunkPos {
        int x = 0;
        int y = 0;

        constexpr static int xSize = 16;
        constexpr static int ySize = 128;
        constexpr static int zSize = 16;

        auto as_tie() const;
        bool operator<(const ChunkPos &rhs) const;
        bool operator==(const ChunkPos &rhs) const;
        ChunkPos &operator+=(const ChunkPos &rhs);
        static ChunkPos toChunkPos(const glm::vec3 &pos);
        glm::vec3 toPos() const;
        float chunkDistance(const glm::vec3 &pos) const;
    };
}// namespace voxie