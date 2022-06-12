//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include "Chunk.h"
#include <RenderingHandler.h>
#include <Scene.h>
#include <glm/glm.hpp>
#include <vector>

namespace voxie {
    class Cube;
    class Handle;
    class Color;
    class Attenuation;
    class Shader;
    class Camera;
    class Position;


    enum class LightType {
        AMBIENT = 0,
        POINT = 1
    };

    struct LightConfig {
        std::shared_ptr<Name> name;
        Handle handle;
        std::shared_ptr<Position> position;
        LightType type;
        std::shared_ptr<Color> color;
        std::shared_ptr<Attenuation> attenuation;
    };

    class LightSource : public NodeWrapper {
    public:
        explicit LightSource(const LightConfig &config);
        ~LightSource();
        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);
        COMPONENT_ADD_FUNCTION(Position);
        COMPONENT_ADD_FUNCTION(Color);
        COMPONENT_ADD_FUNCTION(Attenuation);
        COMPONENT_ADD_FUNCTION(VertexBufferArray);

        const LightType &GetType() const;

    private:
        void OnPositionUpdate(const Position &position);
        LightType type;
    };
}// namespace voxie
