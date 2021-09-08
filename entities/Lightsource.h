//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include "Chunk.h"
#include <RenderingHandler.h>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>

namespace voxie {
    class Shader;
    class Camera;
}// namespace voxie

namespace voxie {
    class Cube;
    class Entity;


    enum class LightType {
        AMBIENT = 0,
        POINT = 1
    };

    struct Atteunation {
        Atteunation(float constant, float linear, float quadratic)
            : constant(constant), linear(linear), quadratic(quadratic) {
        }

        void SetAtteunation(float constant, float linear, float quadratic) {
            this->constant = constant;
            this->linear = linear;
            this->quadratic = quadratic;
        }
        float constant = 0;
        float linear = 0;
        float quadratic = 0;
    };

    struct LightConfig {
        std::shared_ptr<Name> name;
        Entity entity;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Position> position;
        LightType type;
        std::shared_ptr<Cube> cube;
        std::shared_ptr<Color> color;
        std::shared_ptr<Atteunation> atteunation;
    };

    class LightSource {
    public:
        explicit LightSource(const LightConfig& config);
        ~LightSource();
        [[nodiscard]] std::shared_ptr<Position> GetPosition() const;
        [[nodiscard]] std::shared_ptr<Color> GetColor() const;
        [[nodiscard]] const LightType &GetType() const;
        [[nodiscard]] std::shared_ptr<Shader> GetShader() const;
        [[nodiscard]] const Entity &GetEntity() const;
        [[nodiscard]] std::shared_ptr<Atteunation> GetAttuenation() const;
        [[nodiscard]] std::shared_ptr<VertexBufferArray> GetVertexBufferArray() const;

    private:
        Entity entity;
        LightType type;
    };

    std::vector<RenderingConfig> GetRenderingConfigs(const std::shared_ptr<Camera>&, const std::vector<Entity> &);

}// namespace voxie
