//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>
#include <RenderingHandler.h>
#include "Chunk.h"

namespace voxie {
    class Shader;
    class Camera;
}

namespace voxie {
class Cube;
class Entity;


enum class LightType {
    AMBIENT = 0,
    POINT = 1
};

struct Atteunation {
    float constant = 0;
    float linear = 0;
    float quadratic = 0;
};

struct LightConfig {
    std::shared_ptr<Entity> entity;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Position> position;
    LightType type;
    std::shared_ptr<Cube> cube;
    std::shared_ptr<Color> color;
    Atteunation atteunation;
};

class LightSource {
public:
    LightSource(LightConfig config);
    LightSource(Cube* cube, glm::vec3 color, glm::vec3 position);
    const glm::vec3& GetPosition() const;
    const glm::mat4& GetModel() const;
    const glm::vec3& GetColor() const;
    const LightType& GetType() const;
    std::shared_ptr<Shader> GetShader() const;
    const LightConfig& GetConfig() const;

private:
    LightConfig config;
};

class LightSourceHandler {
public:
    LightSourceHandler(std::vector<std::shared_ptr<Shader>> light);
    void AddLight(const LightSource& light);

    std::vector<RenderingConfig> GetRenderingConfigs(std::shared_ptr<Camera>) const;
    const std::vector<LightSource>& GetLightSources() const;
private:
    std::vector<std::shared_ptr<Shader>> lightShaders;
    std::vector<LightSource> lightSources;
};

}
