//
// Created by Stefan Annell on 2022-01-22.
//

#pragma once

#include <memory>

#include "RenderingHandler.h"
#include "EntityComponentSystem.h"
#include "Node.h"
#include "Cube.h"

namespace voxie
{
class Name;
class Shader;
class Position;
class Cube;
class Material;


class CubeEntity : public NodeWrapper {

public:
    CubeEntity(const Handle &, std::shared_ptr<Name>, std::shared_ptr<Shader>, std::shared_ptr<Position>, std::shared_ptr<Material>);
    ~CubeEntity();

    void encode(YAML::Node &) const;
    bool decode(const YAML::Node &);

    [[nodiscard]] std::shared_ptr<Position> GetPosition() const override;
    [[nodiscard]] RenderingConfig GetRenderingConfig() const;
    [[nodiscard]] std::shared_ptr<Shader> GetShader() const;
    [[nodiscard]] std::shared_ptr<Material> GetMaterial() const;
    [[nodiscard]] void RefreshMaterial() const;
    [[nodiscard]] std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;
    [[nodiscard]] const Handle &GetHandle() const override;

private:
    void Init();
    Handle handle;
    Cube cube;
};

}
