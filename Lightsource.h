//
// Created by Stefan Annell on 2021-03-21.
//

#ifndef VOXELENGINE_LIGHTSOURCE_H
#define VOXELENGINE_LIGHTSOURCE_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <vector>

#include "Cube.h"
#include "Shader.h"
#include "Camera.h"

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
    LightSourceHandler(Shader* lightSource, std::vector<Shader*> light);
    void Draw(const Camera& camera) const;
    void AddLight(const LightSource& light);

    std::vector<LightSource>& GetLightSources();
private:
    Shader* lightCubeShader;
    std::vector<Shader*> lightShaders;
    std::vector<LightSource> lightSources;
};

#endif //VOXELENGINE_LIGHTSOURCE_H
