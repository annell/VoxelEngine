//
// Created by Stefan Annell on 2021-04-10.
//

#include "CubeHandler.h"

CubeHandler::CubeHandler(Shader *shader)
 : shader(shader) {

}

void CubeHandler::Draw(const Camera &camera) const {
    for (auto& cube : cubes) {
        cube->SetShaderParameters(*shader);
        cube->Draw();
    }
}

void CubeHandler::AddCube(std::unique_ptr<Cube>&& cube) {
    cubes.push_back(std::move(cube));
}
