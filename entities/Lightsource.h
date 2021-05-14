//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>
#include <RenderingHandler.h>

namespace engine::rendering {
    class Shader;
    class Camera;
}

namespace engine::entities {
class Cube;


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
    LightType type;
    Cube* cube;
    glm::vec3 color;
    glm::vec3 pos;
    Atteunation atteunation;
};

class LightSource {
public:
    LightSource(LightConfig config);
    LightSource(Cube* cube, glm::vec3 color, glm::vec3 position);
    void Draw() const;
    const glm::vec3& GetPosition() const;
    const glm::mat4& GetModel() const;
    const glm::vec3& GetColor() const;
    const LightType& GetType() const;
    const LightConfig& GetConfig() const;
    void SetPosition(const glm::vec3&);

private:
    glm::mat4 model;
    LightConfig config;
};

class LightSourceHandler {
public:
    LightSourceHandler(std::shared_ptr<rendering::Shader> lightSource, std::vector<std::shared_ptr<rendering::Shader>> light);
    void Draw(const rendering::Camera& camera) const;
    void AddLight(const LightSource& light);

    std::vector<LightSource>& GetLightSources();
    std::vector<rendering::RenderingConfig> GetRenderingConfigs(std::shared_ptr<rendering::Camera>) const;
private:
    std::shared_ptr<rendering::Shader> lightCubeShader;
    std::vector<std::shared_ptr<rendering::Shader>> lightShaders;
    std::vector<LightSource> lightSources;
};

}
