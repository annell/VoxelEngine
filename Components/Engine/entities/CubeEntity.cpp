//
// Created by Stefan Annell on 2022-01-22.
//

#include "CubeEntity.h"
#include "Core.h"
#include <GL/glew.h>
#include "Material.h"

namespace voxie {

CubeEntity::CubeEntity(const voxie::Handle& handle, std::shared_ptr<Name> name, std::shared_ptr<Shader> shader, std::shared_ptr<Position> position, std::shared_ptr<Material> material)
    : handle(handle) {
    helper::AddComponent(handle, std::move(name));
    helper::AddComponent(handle, std::move(position));
    helper::AddComponent(handle, std::move(shader));
    helper::AddComponent(handle, std::move(material));

    Init();
}

CubeEntity::~CubeEntity() {
    helper::RemoveComponent<Name>(handle);
    helper::RemoveComponent<Shader>(handle);
    helper::RemoveComponent<Position>(handle);
    helper::RemoveComponent<Material>(handle);
}

void CubeEntity::encode(YAML::Node & node) const {
    node["type"] = "CubeEntity";
    node["id"] = GetHandle().GetId();
    auto name = helper::GetComponent<Name>(handle).get();
    node["name"] = name->name;
    node["position"] = *helper::GetComponent<Position>(handle).get();
    node["material"] = *helper::GetComponent<Material>(handle).get();
}

bool CubeEntity::decode(const YAML::Node & node) {
    GetPosition()->decode(node["position"]);
    GetMaterial()->decode(node["material"]);
    RefreshMaterial();
    return true;
}

void CubeEntity::Init() {
    cube.GenerateVertexAttributes();
    cube.CreateRenderBuffers();
    cube.SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) nullptr);
    cube.SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));
    cube.SetVertexAttrib(1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (6 * sizeof(float)));

    auto shader = GetShader();
    shader->use();
    shader->setMat4("model", GetPosition()->model);
    RefreshMaterial();
}

std::shared_ptr<Position> CubeEntity::GetPosition() const {
    return voxie::helper::GetComponent<Position>(handle);
}

std::shared_ptr<Shader> CubeEntity::GetShader() const {
    return voxie::helper::GetComponent<Shader>(handle);
}

std::shared_ptr<Material> CubeEntity::GetMaterial() const {
    return voxie::helper::GetComponent<Material>(handle);
}

void CubeEntity::RefreshMaterial() const {
    auto shader = GetShader();
    shader->use();

    auto material = GetMaterial();
    std::string index = "0";
    shader->setVec3("materials[" + index + "].ambient", material->ambient);
    shader->setVec3("materials[" + index + "].diffuse", material->diffuse);
    shader->setVec3("materials[" + index + "].specular", material->specular);
    shader->setFloat("materials[" + index + "].shininess", material->shininess);
}

std::shared_ptr<VertexBufferArray> CubeEntity::GetVertexBufferArray() const {
    return cube.GetVertexBufferArray();
}

RenderingConfig CubeEntity::GetRenderingConfig() const {
    return {
            GetShader(),
            GetVertexBufferArray(),
            [&] () {
              auto position = GetPosition();
              auto shader = GetShader();
              shader->use();
              shader->setMat4("model", position->model);
            },
            [] () {},
            [&]() {
              glBindVertexArray(GetVertexBufferArray()->VAO);
              glDrawArrays(GL_TRIANGLES, 0, GetVertexBufferArray()->nrVertex);
            },
            IsEnabled()
    };
}

const Handle &CubeEntity::GetHandle() const {
    return handle;
}

}