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
#include "EntityComponentSystem.h"

namespace voxie {

class Chunk {
public:
    Chunk(std::shared_ptr<Name>, std::shared_ptr<Shader>, std::shared_ptr<Position>);
    ~Chunk();
    void Init();
    void Draw() const;
    void AddCube(ChunkPosition, std::unique_ptr<Cube>&&);
    size_t NrVertex() const;

    std::shared_ptr<Position> GetPosition() const;
    RenderingConfig GetRenderingConfig() const;
    std::shared_ptr<Shader> GetShader() const;
    std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;
    const Entity& GetEntity() const;

private:
    void SetupCubesForRendering();
    void SetupShader();
    void FaceCulling() const;

    size_t nrMaterials = 0;
    std::vector<Cube*> cubesToRender;

    std::map<ChunkPosition, std::unique_ptr<Cube>> cubesMap;
    Entity entity;
};

}
