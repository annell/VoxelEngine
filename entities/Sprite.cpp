//
// Created by Stefan Annell on 2021-08-21.
//

#include "Sprite.h"
#include <Engine.h>
#include <utility>

namespace internal {

}// namespace internal

namespace voxie {

    Sprite::Sprite(const Handle & entity, std::string path, std::shared_ptr<Name> name, std::shared_ptr<Shader> shader, std::shared_ptr<Position2D> position)
        : entity(entity), texture(loadTextureFromFile(std::move(path))), vertexBufferArray(std::move(std::make_shared<VertexBufferArray>())), path(path) {
        helper::AddComponent(entity, std::move(name));
        helper::AddComponent(entity, std::move(position));
        helper::AddComponent(entity, std::move(shader));
        Setup();
    }

    Sprite::~Sprite() {
        helper::RemoveComponent<Name>(entity);
        helper::RemoveComponent<Position2D>(entity);
        helper::RemoveComponent<Shader>(entity);
    }

    void Sprite::encode(YAML::Node &node) const {
        node["type"] = "Sprite";
        node["id"] = GetEntity().GetId();
        node["path"] = path;
        auto name = helper::GetComponent<Name>(entity).get();
        node["name"] = name->name;
        node["position"] = *helper::GetComponent<Position2D>(entity).get();
    }

    bool Sprite::decode(const YAML::Node &node) {
        GetPosition2D()->decode(node["position"]);
        return true;
    }


    void Sprite::Draw() const {
        auto shader = GetShader();
        shader->use();
        shader->setMat4("model", GetPosition2D()->model);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(GetVertexBufferArray()->VAO);
        glDrawArrays(GL_TRIANGLES, 0, GetVertexBufferArray()->nrVertex);
    }

    RenderingConfig Sprite::GetRenderingConfig() const {
        return {
                GetShader(),
                GetVertexBufferArray(),
                []() {},
                []() {},
                [&]() { Draw(); }};
    }

    std::shared_ptr<Position2D> Sprite::GetPosition2D() const {
        return voxie::helper::GetComponent<Position2D>(entity);
    }

    std::shared_ptr<Shader> Sprite::GetShader() const {
        return voxie::helper::GetComponent<Shader>(entity);
    }

    std::shared_ptr<VertexBufferArray> Sprite::GetVertexBufferArray() const {
        return vertexBufferArray;
    }

    const Handle &Sprite::GetEntity() const {
        return entity;
    }

    void Sprite::Setup() const {
        auto vertexBufferArray = GetVertexBufferArray();
        auto pos = GetPosition2D();
        pos->UpdateModel();
        vertexBufferArray->vertexAttributes = {
                //  Position   Texcoords
                pos->pos.x - pos->scale.x, pos->pos.y - pos->scale.y, 0.0f, 1.0f,
                pos->pos.x - pos->scale.x, pos->pos.y + pos->scale.y, 0.0f, 0.0f,
                pos->pos.x + pos->scale.x, pos->pos.y - pos->scale.y, 1.0f, 1.0f,
                pos->pos.x - pos->scale.x, pos->pos.y + pos->scale.y, 0.0f, 0.0f,
                pos->pos.x + pos->scale.x, pos->pos.y + pos->scale.y, 1.0f, 0.0f,
                pos->pos.x + pos->scale.x, pos->pos.y - pos->scale.y, 1.0f, 1.0f};
        vertexBufferArray->nrVertex = 6;

        auto shader = GetShader();
        shader->use();
        shader->setInt("sprite", 0);
        shader->setMat4("model", pos->model);

        vertexBufferArray->CreateBuffers();
        vertexBufferArray->SetVertexAttrib(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        vertexBufferArray->SetVertexAttrib(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
    }
}// namespace voxie
