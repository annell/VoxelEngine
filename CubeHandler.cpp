//
// Created by Stefan Annell on 2021-04-10.
//

#include "CubeHandler.h"

CubeHandler::CubeHandler(Shader *shader)
 : shader(shader) {

}

void CubeHandler::Init() const {
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
    cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    cube->SetVertexAttrib(1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    cubes.push_back(std::move(cube));
}
