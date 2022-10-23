#include "ChunkPos.h"
#include <tuple>

namespace voxie {


    auto ChunkPos::as_tie() const {
        return std::tie(x, y);
    }

    bool ChunkPos::operator<(const ChunkPos &rhs) const {
        return as_tie() < rhs.as_tie();
    }

    bool ChunkPos::operator==(const ChunkPos &rhs) const {
        return as_tie() == rhs.as_tie();
    }

    ChunkPos &ChunkPos::operator+=(const ChunkPos &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    ChunkPos ChunkPos::toChunkPos(const glm::vec3 &pos) {
        return {(int) (pos.x / xSize), (int) (pos.z / zSize)};
    }

    glm::vec3 ChunkPos::toPos() const {
        return glm::vec3{x * xSize, -ySize / 2, y * zSize};
    }

    float ChunkPos::chunkDistance(const glm::vec3 &pos) const {
        auto pos2 = toPos();
        auto power = [](float x1, float x2) {
            return (x2 - x1) * (x2 - x1);
        };
        return sqrt(power(pos.x, pos2.x) + power(pos.z, pos2.z));
    }
}// namespace voxie