//
// Created by Stefan Annell on 2021-04-10.
//

#include "CubeHandler.h"
#include <map>

CubeHandler::CubeHandler(Shader *shader)
 : shader(shader) {

}

void CubeHandler::Init() {
    FaceCulling();
    for (auto& cube : cubes) {
        cube.get()->GenerateVertexAttributes();
        cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        cube->SetVertexAttrib(1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
        if (cube->GetMaterialIndex() > nrMaterials) {
            nrMaterials = cube->GetMaterialIndex();
        }
    }
    shader->use();
    for (auto& cube : cubes) {
        auto& material = cube->GetMaterial();
        std::string index = std::to_string(cube->GetMaterialIndex());
        shader->setVec3("materials["+ index +"].ambient", material.ambient);
        shader->setVec3("materials["+ index +"].diffuse", material.diffuse);
        shader->setVec3("materials["+ index +"].specular", material.specular);
        shader->setFloat("materials["+ index +"].shininess", material.shininess);
    }

    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);  //<--- Not really using it atm, since no dynamic cubes.
                                            // this should be passed to the vertex buffer instead.
}

void CubeHandler::Draw(const Camera &camera) const {
    for (auto& cube : cubes) {
        cube->Draw();
    }
}

void CubeHandler::AddCube(std::unique_ptr<Cube>&& cube) {
    cubesMap[cube->GetChunkPosition()] = cube.get();
    cubes.push_back(std::move(cube));
}

size_t CubeHandler::NrMaterials() const {
    return nrMaterials;
}

size_t CubeHandler::NrVertex() const {
    size_t output = 0;
    for (auto& cube : cubes) {
        output += cube->GetNrVertex();
    }
    return output;
}

void CubeHandler::FaceCulling() const {
    auto render = [&] (Cube* cube, Cube::Face face, const ChunkPosition& pos) {
        auto it = cubesMap.find(pos);
        if (it != cubesMap.end()) {
            cube->SetRenderSide(face, false);
        }
    };
    for (auto& cube : cubes) {
        auto dimensions = cube->GetDimensions();
        auto pos = cube->GetChunkPosition();
        pos.x++;
        render(cube.get(), Cube::Face::RIGHT, pos);
        pos.x--;
        pos.x--;
        render(cube.get(), Cube::Face::LEFT, pos);
        pos.x++;

        pos.y++;
        render(cube.get(), Cube::Face::TOP, pos);
        pos.y--;
        pos.y--;
        render(cube.get(), Cube::Face::BOTTOM, pos);
        pos.y++;

        pos.z++;
        render(cube.get(), Cube::Face::FRONT, pos);
        pos.z--;
        pos.z--;
        render(cube.get(), Cube::Face::BACK, pos);
        pos.z++;
    }
}