//
// Created by Stefan Annell on 2022-01-22.
//

#pragma once

#include <memory>

#include "Cube.h"
#include "EntityComponentSystem.h"
#include "Node.h"
#include "RenderingHandler.h"

namespace voxie {
    class Name;
    class Shader;
    class Position;
    class Cube;
    class Material;
    class RigidBody;
    class Outline;

    class CubeEntity : public NodeWrapper {

    public:
        CubeEntity(const Handle &, std::shared_ptr<Name>, std::shared_ptr<Shader>, std::shared_ptr<Position>, std::shared_ptr<Material>);

        COMPONENT_ADD_FUNCTION(Shader);
        COMPONENT_ADD_FUNCTION(Material);
        COMPONENT_ADD_FUNCTION(Position);
        COMPONENT_ADD_FUNCTION(RigidBody);
        COMPONENT_ADD_FUNCTION(Outline);

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        RenderingConfig GetRenderingConfig() const override;
        void RefreshMaterial() const;
        void Init();

    private:
        Cube cube;
    };

}// namespace voxie
