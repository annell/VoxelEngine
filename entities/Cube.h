//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include <glew.h>
#include <glfw3.h>
#include <vector>

#include "BaseComponents.h"
#include "Shader.h"
#include "VertexBufferArray.h"

namespace voxie {

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
        Cube(const Position &position, Dimensions d, Material m = {}, int materialIndex = -1);
        void GenerateVertexAttributes();
        void CreateRenderBuffers();
        const std::vector<float> &GetVertexAttributes();
        void SetVertexAttrib(GLuint size, GLenum type, GLboolean normalized, GLsizei stride, const void *ptr);
        void SetRenderSide(Face face, bool render);
        [[nodiscard]] const Material &GetMaterial() const;
        [[nodiscard]] const int &GetMaterialIndex() const;
        [[nodiscard]] size_t GetNrVertex() const;
        [[nodiscard]] bool ShouldRender() const;

        [[nodiscard]] std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;

    private:
        void GenerateVertexAttributes(const Side &side);
        void GenerateSides(Position p, Dimensions d);

        Material material;
        int materialIndex;
        std::shared_ptr<VertexBufferArray> vertexBufferArray;
        Side sides[6];
    };

}// namespace voxie
