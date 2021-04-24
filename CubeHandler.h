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
    ~CubeHandler();
    void Init();
    void Draw(const Camera& camera) const;
    void AddCube(std::unique_ptr<Cube>&&);
    size_t NrMaterials() const;
    size_t NrVertex() const;

private:
    void SetupCubesForRendering();
    void SetupShader();
    void FaceCulling() const;

    size_t nrMaterials = 0;
    std::vector<Cube*> cubesToRender;
    Shader* shader;

    std::map<ChunkPosition, std::unique_ptr<Cube>> cubesMap;


    void CreateBuffers(float v[], size_t size);
    void ResetBuffers();

    unsigned int VBO;
    unsigned int VAO;
    std::vector<float> vertexAttributes;
    size_t nrVertex;
};

#endif //VOXELENGINE_CUBEHANDLER_H
