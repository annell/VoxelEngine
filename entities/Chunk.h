//
// Created by Stefan Annell on 2021-04-10.
//

#pragma once

#include "Core.h"
#include <vector>
#include <map>
#include <memory>
#include <RenderingHandler.h>

#include "Shader.h"
#include "Cube.h"

namespace engine::entities {

class Chunk {
public:
    Chunk(std::shared_ptr<rendering::Shader>, Position);
    ~Chunk();
    void Init();
    void Draw() const;
    void AddCube(std::unique_ptr<Cube>&&);
    size_t NrMaterials() const;
    size_t NrVertex() const;

    const Position& GetPosition() const;
    void SetPosition(Position);
    rendering::RenderingConfig GetRenderingConfig() const;
    std::shared_ptr<rendering::Shader> GetShader() const;
    std::shared_ptr<rendering::VertexBufferArray> GetVertexBufferArray() const;

private:
    void SetupCubesForRendering();
    void SetupShader();
    void FaceCulling() const;

    size_t nrMaterials = 0;
    std::vector<Cube*> cubesToRender;
    std::shared_ptr<rendering::Shader> shader;

    std::map<ChunkPosition, std::unique_ptr<Cube>> cubesMap;

    void CreateBuffers(float v[], size_t size);
    void ResetBuffers();

    std::shared_ptr<rendering::VertexBufferArray> vertexBufferArray;

    Position position;
};

}
