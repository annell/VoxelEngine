//
// Created by Stefan Annell on 2021-04-10.
//
#include "Lightsource.h"

LightSource::LightSource(Cube* cubeIn, glm::vec3 colorIn, glm::vec3 positionIn)
 : cube(cubeIn)
 , color(colorIn)
 , pos(positionIn)
 , constant(0)
 , linear(0)
 , quadratic(0)
 , type(0) {
    cube->GenerateVertexAttributes();
    cube->CreateRenderBuffers();
    cube->SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
}

void LightSource::Draw() const {
    cube->Draw();
}

const glm::vec3 & LightSource::GetColor() const {
    return color;
}

const glm::vec3& LightSource::GetPosition() const {
    return pos;
}

const glm::mat4& LightSource::GetModel() const {
    return model;
}

const int& LightSource::GetType() const {
    return type;
}

void LightSource::SetPosition(const glm::vec3& position) {
    pos = position;
    model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, glm::vec3(0.1f));
}

LightSourceHandler::LightSourceHandler(Shader *cubeShader, std::vector<Shader*> light)
 : lightCubeShader(cubeShader)
 , lightShaders(light) {

}

void LightSourceHandler::Draw(const Camera& camera) const {
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
            lightShader->setInt("lights[" + index + "].type", light.GetType());
            lightShader->setFloat("lights[" + index + "].constant", light.constant);
            lightShader->setFloat("lights[" + index + "].linear", light.linear);
            lightShader->setFloat("lights[" + index + "].quadratic", light.quadratic);
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