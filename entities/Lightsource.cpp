//
// Created by Stefan Annell on 2021-04-10.
//
#include <Engine.h>
#include "Lightsource.h"
#include "Core.h"
#include "Cube.h"
#include "Camera.h"

namespace voxie {

LightSource::LightSource(LightConfig config)
 : config(config) {
    voxie::Engine::GetEngine().GetScene().AddEntity(config.entity);
    config.cube->GenerateVertexAttributes();
    config.cube->CreateRenderBuffers();
    config.cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    position = std::make_shared<Position>(config.pos);
    position->model = glm::translate(position->model, position->pos);
    helper::AddComponent(position, *config.entity);
    helper::AddComponent(config.cube->GetVertexBufferArray(), *config.entity);
 }

const glm::vec3 & LightSource::GetColor() const {
    return config.color;
}

const glm::vec3& LightSource::GetPosition() const {
    return position->pos;
}

const glm::mat4& LightSource::GetModel() const {
    return position->model;
}

std::shared_ptr<Shader> LightSource::GetShader() const {
    return config.shader;
}

const LightType& LightSource::GetType() const {
    return config.type;
}

const LightConfig& LightSource::GetConfig() const {
    return config;
}

LightSourceHandler::LightSourceHandler(std::vector<std::shared_ptr<Shader>> light)
 : lightShaders(light) {

}

void LightSourceHandler::AddLight(const LightSource& light) {
    lightSources.push_back(std::move(light));
}

std::vector<RenderingConfig> LightSourceHandler::GetRenderingConfigs(std::shared_ptr<Camera> camera) const {
    for (auto lightShader : lightShaders) {
        lightShader->use();
        lightShader->setInt("nrLights", lightSources.size());
        camera->SetShaderParameters(*lightShader);
        int n = 0;
        for (const auto& light : GetLightSources()) {
            std::string index = std::to_string(n);
            lightShader->setVec3("lights[" + index +"].lightColor", light.GetColor());
            lightShader->setVec3("lights[" + index + "].lightPos", light.GetPosition());
            lightShader->setInt("lights[" + index + "].type", static_cast<int>(light.GetType()));
            lightShader->setFloat("lights[" + index + "].constant", light.GetConfig().atteunation.constant);
            lightShader->setFloat("lights[" + index + "].linear", light.GetConfig().atteunation.linear);
            lightShader->setFloat("lights[" + index + "].quadratic", light.GetConfig().atteunation.quadratic);
            n++;
        }
    }
    std::vector<RenderingConfig> output;
    for (auto& light : lightSources) {
        output.push_back({
            light.GetShader(),
            light.GetConfig().cube->GetVertexBufferArray(),
            [model = light.GetModel(), lightCubeShader = light.GetShader(), camera] () {
                lightCubeShader->use();
                camera->SetShaderParameters(*lightCubeShader);
                lightCubeShader->setMat4("model", model);
            }
        });
    }
    return output;
}

    const std::vector<LightSource> &LightSourceHandler::GetLightSources() const {
        return lightSources;
    }

}
