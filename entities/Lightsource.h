//
// Created by Stefan Annell on 2021-03-21.
//

#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>
#include <RenderingHandler.h>

namespace voxie {
    class Shader;
    class Camera;
}

namespace voxie {
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
    LightSourceHandler(std::shared_ptr<Shader> lightSource, std::vector<std::shared_ptr<Shader>> light);
    void Draw(const Camera& camera) const;
    void AddLight(const LightSource& light);

    std::vector<LightSource>& GetLightSources();
    std::vector<RenderingConfig> GetRenderingConfigs(std::shared_ptr<Camera>) const;
private:
    std::shared_ptr<Shader> lightCubeShader;
    std::vector<std::shared_ptr<Shader>> lightShaders;
    std::vector<LightSource> lightSources;
};

}
