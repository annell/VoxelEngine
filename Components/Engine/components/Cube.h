//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include "components/Material.h"
#include <vector>

namespace voxie {

    class VertexBufferArray;
    class Position;

    struct Dimensions {
        float width;
        float height;
        float depth;
    };

    class Cube {
    public:
        struct Triangle {
            float vertex[9];
        };
        enum Face {
            BACK = 0,
            FRONT = 1,
            LEFT = 2,
            RIGHT = 3,
            BOTTOM = 4,
            TOP = 5,
        };
        struct Side {
            Triangle triangle1;
            Triangle triangle2;
            float normal[3];
            float materialIndex;
            bool render = true;
        };
        Cube();
        Cube(const Position &position, Dimensions d, Material m = {}, int materialIndex = -1);
        void GenerateVertexAttributes();
        void CreateRenderBuffers();
        const std::vector<float> &GetVertexAttributes();
        void SetVertexAttrib(unsigned int size, int type, bool normalized, int stride, const void *ptr);
        void SetRenderSide(Face face, bool render);
        const Material &GetMaterial() const;
        const int &GetMaterialIndex() const;
        size_t GetNrVertex() const;
        bool ShouldRender() const;

        std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;

        void DisableNormals();
        void DisableMaterials();

    private:
        void GenerateVertexAttributes(const Side &side);
        void GenerateSides(Position p, Dimensions d);

        Material material;
        int materialIndex;
        std::shared_ptr<VertexBufferArray> vertexBufferArray;
        Side sides[6];
        bool useNormals = true;
        bool useMaterials = true;
    };

}// namespace voxie
