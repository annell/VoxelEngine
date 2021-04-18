//
// Created by Stefan Annell on 2021-04-10.
//

#ifndef VOXELENGINE_CUBEHANDLER_H
#define VOXELENGINE_CUBEHANDLER_H

#include "Cube.h"
#include "Camera.h"
#include <vector>
#include <map>
#include <memory>

class CubeHandler {
public:
    CubeHandler(Shader* shader);
    void Init();
    void Draw(const Camera& camera) const;
    void AddCube(std::unique_ptr<Cube>&&);
    size_t NrMaterials() const;
    size_t NrVertex() const;

private:
    void FaceCulling() const;

    size_t nrMaterials = 0;
    std::vector<Cube*> cubesToRender;
    Shader* shader;

    std::map<ChunkPosition, std::unique_ptr<Cube>> cubesMap;
};

#endif //VOXELENGINE_CUBEHANDLER_H
