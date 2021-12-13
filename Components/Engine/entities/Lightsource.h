//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include "Chunk.h"
#include <RenderingHandler.h>
#include <Scene.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vector>

namespace voxie {
    class Shader;
    class Camera;
}// namespace voxie

namespace voxie {
    class Cube;
    class Handle;


    enum class LightType {
        AMBIENT = 0,
        POINT = 1
    };

    struct LightConfig {
        std::shared_ptr<Name> name;
        Handle handle;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Position> position;
        LightType type;
        std::shared_ptr<Cube> cube;
        std::shared_ptr<Color> color;
        std::shared_ptr<Attenuation> attenuation;
    };

    class LightSource : public NodeWrapper {
    public:
        explicit LightSource(const LightConfig &config);
        ~LightSource();
        void encode(YAML::Node &) const;
        bool decode(const YAML::Node &);
        [[nodiscard]] std::shared_ptr<Position> GetPosition() const override;
        [[nodiscard]] std::shared_ptr<Color> GetColor() const;
        [[nodiscard]] const LightType &GetType() const;
        [[nodiscard]] std::shared_ptr<Shader> GetShader() const;
        [[nodiscard]] const Handle &GetHandle() const override;
        [[nodiscard]] std::shared_ptr<Attenuation> GetAttenuation() const;
        [[nodiscard]] std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;

    private:
        Handle handle;
        LightType type;
        std::shared_ptr<Cube> cube;
    };

    std::vector<RenderingConfig> GetRenderingConfigs(const std::shared_ptr<Camera> &, const Scene::SceneEntities &);

}// namespace voxie
