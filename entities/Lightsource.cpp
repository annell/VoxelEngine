//
// Created by Stefan Annell on 2021-04-10.
//
#include "Lightsource.h"
#include "Core.h"
#include "Cube.h"
#include "Camera.h"

namespace engine::entities {

LightSource::LightSource(LightConfig config)
 : config(config) {
    config.cube->GenerateVertexAttributes();
    config.cube->CreateRenderBuffers();
    config.cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, config.pos);
 }

void LightSource::Draw() const {
    config.cube->Draw();
}

const glm::vec3 & LightSource::GetColor() const {
    return config.color;
}

const glm::vec3& LightSource::GetPosition() const {
    return config.pos;
}

const glm::mat4& LightSource::GetModel() const {
    return model;
}

const LightType& LightSource::GetType() const {
    return config.type;
}

const LightConfig& LightSource::GetConfig() const {
    return config;
}

void LightSource::SetPosition(const glm::vec3& position) {
    config.pos = position;
    model = glm::mat4(1.0f);
    model = glm::translate(model, config.pos);
    model = glm::scale(model, glm::vec3(0.1f));
}

LightSourceHandler::LightSourceHandler(std::shared_ptr<rendering::Shader> cubeShader, std::vector<std::shared_ptr<rendering::Shader>> light)
 : lightCubeShader(cubeShader)
 , lightShaders(light) {

}

void LightSourceHandler::Draw(const rendering::Camera& camera) const {
    lightCubeShader->use();
    camera.SetShaderParameters(*lightCubeShader);
    for (auto &light : lightSources) {
        lightCubeShader->setMat4("model", light.GetModel());
        light.Draw();
    }

    for (auto lightShader : lightShaders) {
        lightShader->use();
        lightShader->setInt("nrLights", lightSources.size());
        camera.SetShaderParameters(*lightShader);
        int n = 0;
        for (auto& light : lightSources) {
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
}

void LightSourceHandler::AddLight(const LightSource& light) {
    lightSources.push_back(std::move(light));
}

std::vector<LightSource> & LightSourceHandler::GetLightSources() {
    return lightSources;
}

std::vector<rendering::RenderingConfig> LightSourceHandler::GetRenderingConfigs(std::shared_ptr<rendering::Camera> camera) const {
    for (auto lightShader : lightShaders) {
        lightShader->use();
        lightShader->setInt("nrLights", lightSources.size());
        camera->SetShaderParameters(*lightShader);
        int n = 0;
        for (auto& light : lightSources) {
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
    std::vector<rendering::RenderingConfig> output;
    for (auto& light : lightSources) {
        output.push_back({
            lightCubeShader,
            light.GetConfig().cube->GetVertexBufferArray(),
            [model = light.GetModel(), lightCubeShader = lightCubeShader, camera] () {
                lightCubeShader->use();
                camera->SetShaderParameters(*lightCubeShader);
                lightCubeShader->setMat4("model", model);
            }
        });
    }
    return output;
}

}
