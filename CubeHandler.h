//
// Created by Stefan Annell on 2021-04-10.
//

#ifndef VOXELENGINE_CUBEHANDLER_H
#define VOXELENGINE_CUBEHANDLER_H

#include "Cube.h"
#include "Camera.h"
#include <vector>
#include <memory>

class CubeHandler {
public:
    CubeHandler(Shader* shader);
    void Init() const;
    void Draw(const Camera& camera) const;
    void AddCube(std::unique_ptr<Cube>&&);

private:
    std::vector<std::unique_ptr<Cube>> cubes;
    Shader* shader;
};

#endif //VOXELENGINE_CUBEHANDLER_H
