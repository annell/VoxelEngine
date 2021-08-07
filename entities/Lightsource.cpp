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
 : entity(config.entity)
 , type(config.type) {
    voxie::Engine::GetEngine().GetScene().AddEntity(entity);
    config.cube->GenerateVertexAttributes();
    config.cube->CreateRenderBuffers();
    config.cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    config.position->model = glm::translate(config.position->model, config.position->pos);
    config.position->SetPosition(config.position->pos);
    helper::AddComponent(entity, config.name);
    helper::AddComponent(entity, config.position);
    helper::AddComponent(entity, config.cube->GetVertexBufferArray());
    helper::AddComponent(entity, config.shader);
    helper::AddComponent(entity, config.color);
    if (type == LightType::POINT) {
        helper::AddComponent(entity, config.atteunation);
    }
 }

std::shared_ptr<Color> LightSource::GetColor() const {
    return voxie::helper::GetComponent<Color>(entity);
}

std::shared_ptr<Position> LightSource::GetPosition() const {
    return voxie::helper::GetComponent<Position>(entity);
}

std::shared_ptr<Shader> LightSource::GetShader() const {
    return voxie::helper::GetComponent<Shader>(entity);
}

const LightType& LightSource::GetType() const {
    return type;
}

std::shared_ptr<Atteunation> LightSource::GetAttuenation()const {
    return voxie::helper::GetComponent<Atteunation>(entity);
}

std::shared_ptr<VertexBufferArray> LightSource::GetVertexBufferArray() const {
    return voxie::helper::GetComponent<VertexBufferArray>(entity);
}

void LightSourceHandler::AddLight(std::unique_ptr<LightSource>&& light) {
    lightSources.push_back(std::move(light));
}

std::vector<RenderingConfig> LightSourceHandler::GetRenderingConfigs(std::shared_ptr<Camera> camera) const {
    for (auto shader : shaders) {
        shader->use();
        shader->setInt("nrLights", lightSources.size());
        camera->SetShaderParameters(*shader);
        int n = 0;
        for (const auto& light : lightSources) {
            std::string index = std::to_string(n);
            shader->setVec3("lights[" + index +"].lightColor", light->GetColor()->color);
            shader->setVec3("lights[" + index + "].lightPos", light->GetPosition()->pos);
            shader->setInt("lights[" + index + "].type", static_cast<int>(light->GetType()));
            if (light->GetType() == LightType::POINT) {
                shader->setFloat("lights[" + index + "].constant", light->GetAttuenation()->constant);
                shader->setFloat("lights[" + index + "].linear", light->GetAttuenation()->linear);
                shader->setFloat("lights[" + index + "].quadratic", light->GetAttuenation()->quadratic);
            }
            n++;
        }
    }
    std::vector<RenderingConfig> output;
    for (auto& light : lightSources) {
        output.push_back({
            light->GetShader(),
            light->GetVertexBufferArray(),
            [model = light->GetPosition()->model, lightCubeShader = light->GetShader(), camera] () {
                lightCubeShader->use();
                camera->SetShaderParameters(*lightCubeShader);
                lightCubeShader->setMat4("model", model);
            }
        });
    }
    return output;
}

void LightSourceHandler::AddShader(std::shared_ptr<Shader> shader) {
    shaders.push_back(shader);
}

}
