//
// Created by Stefan Annell on 2021-04-10.
//
#include "Core.h"
#include "Lightsource.h"
#include "Camera.h"
#include "Cube.h"
#include <Engine.h>
#include "Shader.h"

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace voxie {

    LightSource::LightSource(const LightConfig &config)
        : handle(config.handle), type(config.type), cube(std::move(config.cube)) {
        cube->GenerateVertexAttributes();
        cube->CreateRenderBuffers();
        cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);
        config.position->model = glm::translate(config.position->model, config.position->pos);
        config.position->SetPosition(config.position->pos);
        helper::AddComponent(handle, config.name);
        helper::AddComponent(handle, config.position);
        helper::AddComponent(handle, config.shader);
        helper::AddComponent(handle, config.color);
        if (type == LightType::POINT) {
            helper::AddComponent(handle, config.attenuation);
        }
    }

    LightSource::~LightSource() {
        helper::RemoveComponent<Name>(handle);
        helper::RemoveComponent<Position>(handle);
        helper::RemoveComponent<Shader>(handle);
        helper::RemoveComponent<Color>(handle);
        if (GetAttenuation()) {
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

    std::shared_ptr<Color> LightSource::GetColor() const {
        return voxie::helper::GetComponent<Color>(handle);
    }

    std::shared_ptr<Position> LightSource::GetPosition() const {
        return voxie::helper::GetComponent<Position>(handle);
    }

    std::shared_ptr<Shader> LightSource::GetShader() const {
        return voxie::helper::GetComponent<Shader>(handle);
    }

    const LightType &LightSource::GetType() const {
        return type;
    }

    std::shared_ptr<Attenuation> LightSource::GetAttenuation() const {
        return voxie::helper::GetComponent<Attenuation>(handle);
    }

    std::shared_ptr<VertexBufferArray> LightSource::GetVertexBufferArray() const {
        return voxie::helper::GetComponent<VertexBufferArray>(handle);
    }

    const Handle &LightSource::GetHandle() const {
        return handle;
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

        return {};
    }

}// namespace voxie
