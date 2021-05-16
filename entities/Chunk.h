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
    Chunk(std::string name, std::shared_ptr<Shader>, std::shared_ptr<Position>);
    ~Chunk();
    void Init();
    void Draw() const;
    void AddCube(std::unique_ptr<Cube>&&);
    size_t NrMaterials() const;
    size_t NrVertex() const;

    std::shared_ptr<Position> GetPosition() const;
    void SetPosition(Position);
    RenderingConfig GetRenderingConfig() const;
    std::shared_ptr<Shader> GetShader() const;
    std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;
    std::shared_ptr<Entity> GetEntity() const;

private:
    void SetupCubesForRendering();
    void SetupShader();
    void FaceCulling() const;

    size_t nrMaterials = 0;
    std::vector<Cube*> cubesToRender;

    std::map<ChunkPosition, std::unique_ptr<Cube>> cubesMap;
    std::shared_ptr<Entity> entity;
};

}
