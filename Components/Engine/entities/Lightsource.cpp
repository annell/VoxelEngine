//
// Created by Stefan Annell on 2021-04-10.
//
#include "Core.h"
#include "Lightsource.h"
#include "Camera.h"
#include "Cube.h"
#include "CubeEntity.h"
#include <Engine.h>
#include "Shader.h"

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace voxie {

    LightSource::LightSource(const LightConfig &config)
        : NodeWrapper(config.handle), type(config.type), cube(std::move(config.cube)) {
        cube->GenerateVertexAttributes();
        cube->CreateRenderBuffers();
        cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);
        config.position->model = glm::translate(config.position->model, config.position->pos);
        config.position->SetPosition(config.position->pos);
        COMPONENT_REGISTER(Position, config.position);
        COMPONENT_REGISTER(Shader, config.shader);
        COMPONENT_REGISTER(Name, config.name);
        COMPONENT_REGISTER(Color, config.color);
        if (type == LightType::POINT) {
            helper::AddComponent(handle, config.attenuation);
        }
    }

    LightSource::~LightSource() {
        if (helper::HasComponent<Attenuation>(handle)) {
            helper::RemoveComponent<Attenuation>(handle);
        }
    }

    void LightSource::encode(YAML::Node &node) const {
        node["type"] = "LightSource";
        node["id"] = GetHandle().GetId();
        node["lightType"] = (int) GetType();
        auto name = helper::GetComponent<Name>(handle).get();
        node["name"] = name->name;
        node["position"] = *helper::GetComponent<Position>(handle).get();
        node["color"] = *helper::GetComponent<Color>(handle).get();
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

    const LightType &LightSource::GetType() const {
        return type;
    }

    std::vector<RenderingConfig> GetRenderingConfigs(const std::shared_ptr<Camera> &camera, const Scene::SceneEntities &entities) {
        auto lightSources = helper::GetSceneNodes<LightSource>(entities);

        for (const auto &chunk : helper::GetSceneNodes<Chunk>(entities)) {
            auto shader = chunk->GetShader();
            shader->use();
            shader->setInt("nrLights", lightSources.size());
            shader->setBool("selected", chunk->GetHandle() == camera->GetSelection());
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

        for (const auto &cube : helper::GetSceneNodes<CubeEntity>(entities)) {
            auto shader = cube->GetShader();
            shader->use();
            shader->setInt("nrLights", lightSources.size());
            shader->setBool("selected", cube->GetHandle() == camera->GetSelection());
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

        return {};
    }

}// namespace voxie
