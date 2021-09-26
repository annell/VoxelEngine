//
// Created by Stefan Annell on 2021-04-10.
//
#include "Lightsource.h"
#include "Camera.h"
#include "Core.h"
#include "Cube.h"
#include <Engine.h>

namespace voxie {

    LightSource::LightSource(const LightConfig &config)
        : entity(config.entity), type(config.type), cube(std::move(config.cube)) {
        cube->GenerateVertexAttributes();
        cube->CreateRenderBuffers();
        cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);
        config.position->model = glm::translate(config.position->model, config.position->pos);
        config.position->SetPosition(config.position->pos);
        helper::AddComponent(entity, config.name);
        helper::AddComponent(entity, config.position);
        helper::AddComponent(entity, config.shader);
        helper::AddComponent(entity, config.color);
        if (type == LightType::POINT) {
            helper::AddComponent(entity, config.attenuation);
        }
    }

    LightSource::~LightSource() {
        helper::RemoveComponent<Name>(entity);
        helper::RemoveComponent<Position>(entity);
        helper::RemoveComponent<Shader>(entity);
        helper::RemoveComponent<Color>(entity);
        if (GetAttenuation()) {
            helper::RemoveComponent<Attenuation>(entity);
        }
    }

    void LightSource::encode(YAML::Node &node) const {
        node["type"] = "LightSource";
        node["lightType"] = (int) GetType();
        auto name = helper::GetComponent<Name>(entity).get();
        node["name"] = name->name;
        node["position"] = *helper::GetComponent<Position>(entity).get();
        node["color"] = *helper::GetComponent<Color>(entity).get();
        if (auto attenuation = GetAttenuation()) {
            node["attenuation"] = *attenuation;
        }
    }

    bool LightSource::decode(const YAML::Node &node) {
        GetPosition()->decode(node["position"]);
        GetColor()->decode(node["color"]);
        if (node["attenuation"].IsDefined()) {
            GetAttenuation()->decode(node["attenuation"]);
        }
        return true;
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

    const LightType &LightSource::GetType() const {
        return type;
    }

    std::shared_ptr<Attenuation> LightSource::GetAttenuation() const {
        return voxie::helper::GetComponent<Attenuation>(entity);
    }

    std::shared_ptr<VertexBufferArray> LightSource::GetVertexBufferArray() const {
        return voxie::helper::GetComponent<VertexBufferArray>(entity);
    }

    const Entity &LightSource::GetEntity() const {
        return entity;
    }

    std::vector<RenderingConfig> GetRenderingConfigs(const std::shared_ptr<Camera> &camera, const std::vector<Entity> &entities) {
        auto lightSources = helper::GetComponents<LightSource>(entities);
        for (const auto &shader : helper::GetComponents<Shader>(entities)) {
            shader->use();
            shader->setInt("nrLights", lightSources.size());
            camera->SetShaderParameters(*shader);
            int n = 0;
            for (const auto &light : lightSources) {
                std::string index = std::to_string(n);
                shader->setVec3("lights[" + index + "].lightColor", light->GetColor()->color);
                shader->setVec3("lights[" + index + "].lightPos", light->GetPosition()->pos);
                shader->setInt("lights[" + index + "].type", static_cast<int>(light->GetType()));
                if (light->GetAttenuation()) {
                    shader->setFloat("lights[" + index + "].constant", light->GetAttenuation()->constant);
                    shader->setFloat("lights[" + index + "].linear", light->GetAttenuation()->linear);
                    shader->setFloat("lights[" + index + "].quadratic", light->GetAttenuation()->quadratic);
                }
                n++;
            }
        }

        std::vector<RenderingConfig> output;
        for (auto &light : lightSources) {
            output.push_back({light->GetShader(),
                              light->GetVertexBufferArray(),
                              [model = light->GetPosition()->model, lightCubeShader = light->GetShader(), camera]() {
                                  lightCubeShader->use();
                                  camera->SetShaderParameters(*lightCubeShader);
                                  lightCubeShader->setMat4("model", model);
                              }});
        }
        return output;
    }

}// namespace voxie
