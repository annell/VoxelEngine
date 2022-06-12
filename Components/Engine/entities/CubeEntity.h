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

    class CubeEntity : public NodeWrapper {

    public:
        CubeEntity(const Handle &, std::shared_ptr<Name>, std::shared_ptr<Shader>, std::shared_ptr<Position>, std::shared_ptr<Material>);
        ~CubeEntity();

        COMPONENT_ADD_FUNCTION(Shader);
        COMPONENT_ADD_FUNCTION(Material);
        COMPONENT_ADD_FUNCTION(Position);

        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);

        RenderingConfig GetRenderingConfig() const override;
        void RefreshMaterial() const;

    private:
        void Init();
        Cube cube;
    };

}// namespace voxie
