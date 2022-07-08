//
// Created by Stefan Annell on 2022-01-22.
//

#include "CubeEntity.h"
#include "Core.h"
#include <GL/glew.h>

namespace voxie {

    CubeEntity::CubeEntity(const voxie::Handle &handle, std::shared_ptr<Name> name, std::shared_ptr<Shader> shader, std::shared_ptr<Position> position, std::shared_ptr<Material> material)
        : NodeWrapper(handle) {

        COMPONENT_REGISTER(Material, material);
        COMPONENT_REGISTER(RigidBody, std::make_shared<RigidBody>(*position.get()));
        COMPONENT_REGISTER(Position, position);
        COMPONENT_REGISTER(Shader, shader);
        COMPONENT_REGISTER(Name, name);
        COMPONENT_REGISTER(Outline, std::make_shared<Outline>());

        cube.GenerateVertexAttributes();
        cube.CreateRenderBuffers();
        cube.SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (0 * sizeof(float)));
        cube.SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));
        cube.SetVertexAttrib(1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (6 * sizeof(float)));
        Init();
    }

    void CubeEntity::encode(YAML::Node &node) const {
        node["type"] = "CubeEntity";
        node["id"] = GetHandle().GetId();
        auto name = helper::GetComponent<Name>(handle).get();
        node["name"] = name->name;
        node["position"] = *helper::GetComponent<Position>(handle).get();
        node["material"] = *helper::GetComponent<Material>(handle).get();
        node["rigidBody"] = *helper::GetComponent<RigidBody>(handle).get();
    }

    bool CubeEntity::decode(const YAML::Node &node) {
        GetPosition()->decode(node["position"]);
        GetMaterial()->decode(node["material"]);
        auto rigidBody = GetRigidBody();
        rigidBody->decode(node["rigidBody"]);
        Init();
        return true;
    }

    void CubeEntity::Init() {
        auto shader = GetShader();
        shader->use();
        shader->setMat4("model", GetPosition()->model);
        RefreshMaterial();

        auto outline = GetOutline();

        outline->shader = std::make_shared<voxie::Shader>(
                std::map<std::string, unsigned int>{
                        std::make_pair(BASE_PATH + SHADERS + "/outline.vs", GL_VERTEX_SHADER),
                        std::make_pair(BASE_PATH + SHADERS + "/outline.fs", GL_FRAGMENT_SHADER)});
        outline->vertexBufferArray = cube.GetVertexBufferArray();
        outline->position = GetPosition();

        auto rigidBody = GetRigidBody();
        rigidBody->collider = CreateBoxCollider(rigidBody->rigidBody, *GetPosition());
        rigidBody->SetPosition(*GetPosition());
    }

    void CubeEntity::RefreshMaterial() const {
        auto shader = GetShader();
        shader->use();

        std::string index = "0";
        shader->setVec3("materials[" + index + "].ambient", GetMaterial()->ambient);
        shader->setVec3("materials[" + index + "].diffuse", GetMaterial()->diffuse);
        shader->setVec3("materials[" + index + "].specular", GetMaterial()->specular);
        shader->setFloat("materials[" + index + "].shininess", GetMaterial()->shininess);
        shader->setInt("material", 0);
    }

    RenderingConfig CubeEntity::GetRenderingConfig() const {
        return {
                GetShader(),
                cube.GetVertexBufferArray(),
                [&]() {
                    auto position = GetPosition();
                    auto shader = GetShader();
                    shader->use();
                    shader->setMat4("model", position->model);
                },
                [&]() {
                    GetOutline()->Render();
                },
                [&]() {
                    glBindVertexArray(cube.GetVertexBufferArray()->VAO);
                    glDrawArrays(GL_TRIANGLES, 0, cube.GetVertexBufferArray()->nrVertex);
                },
                IsEnabled()};
    }

}// namespace voxie
