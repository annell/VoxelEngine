//
// Created by Stefan Annell on 2021-04-10.
//
#include "Lightsource.h"
#include "Camera.h"
#include "Core.h"
#include "Cube.h"
#include "CubeEntity.h"
#include "Shader.h"
#include <Engine.h>

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace voxie {

    LightSource::LightSource(const LightConfig &config)
        : NodeWrapper(config.handle), type(config.type) {
        config.position->model = glm::translate(config.position->model, config.position->pos);
        config.position->SetPosition(config.position->pos);
        config.position->onUpdate.Bind([]() { Engine::GetEngine().GetScene().UpdateLights(); });
        config.color->onUpdate.Bind([]() { Engine::GetEngine().GetScene().UpdateLights(); });

        COMPONENT_REGISTER(Position, config.position);
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

    void LightSource::OnPositionUpdate(const Position &) {
    }

}// namespace voxie
