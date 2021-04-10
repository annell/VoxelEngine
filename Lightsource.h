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

class LightSource {
public:
    LightSource(Cube* cube, glm::vec3 color, glm::vec3 position);
    void Draw() const;
    const glm::vec3& GetPosition() const;
    const glm::mat4& GetModel() const;
    void SetPosition(const glm::vec3&);
    const glm::vec3& GetColor() const;

private:
    Cube* cube;
    glm::vec3 color;
    glm::vec3 pos;
    glm::mat4 model;
};

class LightSourceHandler {
public:
    LightSourceHandler(Shader* lightSource, Shader* light);
    void Draw(const Camera& camera) const;
    void AddLight(const LightSource& light);

    std::vector<LightSource>& GetLightSources();
private:
    Shader* lightCubeShader;
    Shader* lightShader;
    std::vector<LightSource> lightSources;
};

#endif //VOXELENGINE_LIGHTSOURCE_H
