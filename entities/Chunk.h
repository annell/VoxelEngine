//
// Created by Stefan Annell on 2021-04-10.
//

#ifndef VOXELENGINE_CUBEHANDLER_H
#define VOXELENGINE_CUBEHANDLER_H

#include "Cube.h"
#include "../rendering/Camera.h"
#include <vector>
#include <map>
#include <memory>

class Chunk {
public:
    Chunk(Shader, Position);
    ~Chunk();
    void Init();
    void Draw() const;
    void AddCube(std::unique_ptr<Cube>&&);
    size_t NrMaterials() const;
    size_t NrVertex() const;

    const Position& GetPosition() const;
    void SetPosition(Position);
    Shader* GetShader();

private:
    void SetupCubesForRendering();
    void SetupShader();
    void FaceCulling() const;

    size_t nrMaterials = 0;
    std::vector<Cube*> cubesToRender;
    Shader shader;

    std::map<ChunkPosition, std::unique_ptr<Cube>> cubesMap;

    void CreateBuffers(float v[], size_t size);
    void ResetBuffers();

    unsigned int VBO;
    unsigned int VAO;
    std::vector<float> vertexAttributes;
    size_t nrVertex = 0;

    Position position;
};

#endif //VOXELENGINE_CUBEHANDLER_H
