//
// Created by Stefan Annell on 2021-08-21.
//

#include "Sprite.h"
#include "Shader.h"
#include "VertexBufferArray.h"
#include "components/BaseComponents.h"
#include <Engine.h>
#include <utility>

namespace internal {

}// namespace internal

namespace voxie {

    Sprite::Sprite(const Handle &handle, std::string path, std::shared_ptr<Name> name, std::shared_ptr<Shader> shader, std::shared_ptr<Position2D> position)
        : NodeWrapper(handle), texture(loadTextureFromFile(std::move(path))), vertexBufferArray(std::move(std::make_shared<VertexBufferArray>())), path(path) {
        COMPONENT_REGISTER(Position2D, position);
        COMPONENT_REGISTER(Shader, shader);
        COMPONENT_REGISTER(Name, name);
        Setup();
    }

    Sprite::~Sprite() {
    }

    void Sprite::encode(YAML::Node &node) const {
        node["type"] = "Sprite";
        node["id"] = GetHandle().GetId();
        node["path"] = path;
        auto name = helper::GetComponent<Name>(handle).get();
        node["name"] = name->name;
        node["position"] = *helper::GetComponent<Position2D>(handle).get();
    }

    bool Sprite::decode(const YAML::Node &node) {
        DecodeComponent(node["position"], GetPosition2D());
        return true;
    }


    void Sprite::Draw() const {
        auto shader = GetShader();
        shader->use();
        shader->setMat4("model", GetPosition2D()->model);

        RenderingInterface::BindTexture(RenderingType::TextureType2D, texture);
        RenderingInterface::Draw(GetVertexBufferArray());
    }

    RenderingConfig Sprite::GetRenderingConfig() const {
        return {
                GetShader(),
                GetVertexBufferArray(),
                []() {},
                []() {},
                [&]() { Draw(); }};
    }

    std::shared_ptr<VertexBufferArray> Sprite::GetVertexBufferArray() const {
        return vertexBufferArray;
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
        vertexBufferArray->SetVertexAttrib(2, 4 * sizeof(float), (void *) 0);
        vertexBufferArray->SetVertexAttrib(2, 4 * sizeof(float), (void *) (2 * sizeof(float)));
    }
}// namespace voxie
