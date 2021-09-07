//
// Created by Stefan Annell on 2021-08-21.
//

#include "Sprite.h"
#include "stb_image.h"
#include <Engine.h>
#include <SOIL.h>


namespace internal {
    voxie::Sprite::Texture2D loadTextureFromFile(std::string file) {
        int width, height, nrChannels;

        voxie::Sprite::Texture2D texture;
        glGenTextures(1, &texture);
        assert(texture && "Can't be 0");
        glBindTexture(GL_TEXTURE_2D, texture);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *image = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
        assert(image && "No image found!");

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

        stbi_image_free(image);

        return texture;
    }

}// namespace internal

namespace voxie {

    Sprite::Sprite(std::string path, std::shared_ptr<Name> name, std::shared_ptr<Shader> shader, std::shared_ptr<Position2D> position)
        : entity(Entity::MakeEntity()), texture(internal::loadTextureFromFile(path)) {
        helper::AddComponent(entity, name);
        helper::AddComponent(entity, position);
        helper::AddComponent(entity, shader);
        helper::AddComponent(entity, std::make_shared<VertexBufferArray>());
        Setup();
    }

    void Sprite::Draw() const {
        auto shader = GetShader();
        shader->use();
        shader->setMat4("model", GetPosition()->model);

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

    std::shared_ptr<Position2D> Sprite::GetPosition() const {
        return voxie::helper::GetComponent<Position2D>(entity);
    }

    std::shared_ptr<Shader> Sprite::GetShader() const {
        return voxie::helper::GetComponent<Shader>(entity);
    }

    std::shared_ptr<VertexBufferArray> Sprite::GetVertexBufferArray() const {
        return voxie::helper::GetComponent<VertexBufferArray>(entity);
    }

    const Entity &Sprite::GetEntity() const {
        return entity;
    }

    void Sprite::Setup() const {
        auto vertexBufferArray = GetVertexBufferArray();
        auto pos = GetPosition();
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
