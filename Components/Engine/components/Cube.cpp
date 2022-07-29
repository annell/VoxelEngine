//
// Created by Stefan Annell on 2021-05-18.
//
#include "Cube.h"
#include "VertexBufferArray.h"
#include "components/Position.h"

namespace voxie {

    Cube::Cube()
        : Cube(Position{0, 0, 0}, Dimensions{1, 1, 1}, {}, 0) {
    }

    Cube::Cube(const Position &position, Dimensions dimensions, Material material, int materialIndex)
        : material(material), materialIndex(materialIndex), vertexBufferArray(std::make_shared<VertexBufferArray>()) {
        GenerateSides(position, dimensions);
    }

    void Cube::GenerateVertexAttributes() {
        vertices.clear();
        for (auto side : sides) {
            if (side.render) {
                GenerateVertexAttributes(side);
                vertexBufferArray->nrVertex += 6;
            }
        }
    }

    void Cube::CreateRenderBuffers() {
        if (ShouldRender()) {
            vertexBufferArray->CreateBuffers();
        }
    }

    const std::vector<float> &Cube::GetVertexAttributes() {
        return vertexBufferArray->vertexAttributes;
    }

    void Cube::SetVertexAttrib(unsigned int size, int stride, const void *ptr) {
        vertexBufferArray->SetVertexAttrib(size, stride, ptr);
    }

    void Cube::SetRenderSide(Face face, bool render) {
        sides[face].render = render;
    }

    const Material &Cube::GetMaterial() const {
        return material;
    }

    const int &Cube::GetMaterialIndex() const {
        return materialIndex;
    }

    size_t Cube::GetNrVertex() const {
        return vertexBufferArray->nrVertex;
    }

    bool Cube::ShouldRender() const {
        return GetNrVertex() != 0;
    }

    std::shared_ptr<VertexBufferArray> Cube::GetVertexBufferArray() const {
        return vertexBufferArray;
    }

    void Cube::GenerateVertexAttributes(const Side &side) {
        const auto addAttributes = [&, useNormals = useNormals, useMaterials = useMaterials](const Triangle &triangle, const float normals[3], float materialIndex, std::vector<float> &vertexAttributes) {
            int index = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    vertexAttributes.push_back(triangle.vertex[index]);
                    vertices.push_back(triangle.vertex[index]);
                    index++;
                }
                if (useNormals) {
                    for (int j = 0; j < 3; j++) {
                        vertexAttributes.push_back(normals[j]);
                    }
                }
                if (useMaterials) {
                    vertexAttributes.push_back(materialIndex);
                }
            }
        };
        addAttributes(side.triangle1, side.normal, side.materialIndex, vertexBufferArray->vertexAttributes);
        addAttributes(side.triangle2, side.normal, side.materialIndex, vertexBufferArray->vertexAttributes);
    }

    void Cube::GenerateSides(Position p, Dimensions d) {
        sides[Face::BACK].materialIndex = materialIndex;
        sides[Face::BACK].normal[0] = 0;
        sides[Face::BACK].normal[1] = 0;
        sides[Face::BACK].normal[2] = -1;
        sides[Face::BACK].triangle1 = {
                p.pos[0] - (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] - (d.depth) / 2};
        sides[Face::BACK].triangle2 = {
                p.pos[0] + (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] - (d.depth) / 2};

        sides[Face::FRONT].materialIndex = materialIndex;
        sides[Face::FRONT].normal[0] = 0;
        sides[Face::FRONT].normal[1] = 0;
        sides[Face::FRONT].normal[2] = 1;
        sides[Face::FRONT].triangle1 = {
                p.pos[0] - (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] + (d.depth) / 2};
        sides[Face::FRONT].triangle2 = {
                p.pos[0] + (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] + (d.depth) / 2};

        sides[Face::LEFT].materialIndex = materialIndex;
        sides[Face::LEFT].normal[0] = -1;
        sides[Face::LEFT].normal[1] = 0;
        sides[Face::LEFT].normal[2] = 0;
        sides[Face::LEFT].triangle1 = {
                p.pos[0] - (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] - (d.depth) / 2};
        sides[Face::LEFT].triangle2 = {
                p.pos[0] - (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] + (d.depth) / 2};

        sides[Face::RIGHT].materialIndex = materialIndex;
        sides[Face::RIGHT].normal[0] = 1;
        sides[Face::RIGHT].normal[1] = 0;
        sides[Face::RIGHT].normal[2] = 0;
        sides[Face::RIGHT].triangle1 = {
                p.pos[0] + (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] - (d.depth) / 2};
        sides[Face::RIGHT].triangle2 = {
                p.pos[0] + (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] + (d.depth) / 2};

        sides[Face::BOTTOM].materialIndex = materialIndex;
        sides[Face::BOTTOM].normal[0] = 0;
        sides[Face::BOTTOM].normal[1] = -1;
        sides[Face::BOTTOM].normal[2] = 0;
        sides[Face::BOTTOM].triangle1 = {
                p.pos[0] - (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] - (d.depth) / 2};
        sides[Face::BOTTOM].triangle2 = {
                p.pos[0] + (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] - (d.height) / 2, p.pos[2] + (d.depth) / 2};

        sides[Face::TOP].materialIndex = materialIndex;
        sides[Face::TOP].normal[0] = 0;
        sides[Face::TOP].normal[1] = 1;
        sides[Face::TOP].normal[2] = 0;
        sides[Face::TOP].triangle1 = {
                p.pos[0] - (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] - (d.depth) / 2,
                p.pos[0] + (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] + (d.depth) / 2};
        sides[Face::TOP].triangle2 = {
                p.pos[0] + (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] + (d.depth) / 2,
                p.pos[0] - (d.width) / 2, p.pos[1] + (d.height) / 2, p.pos[2] - (d.depth) / 2};
    }
    void Cube::DisableNormals() {
        useNormals = false;
    }
    void Cube::DisableMaterials() {
        useMaterials = false;
    }
    const std::vector<float> &Cube::GetVertices() const {
        return vertices;
    }

}// namespace voxie
