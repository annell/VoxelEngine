//
// Created by Stefan Annell on 2021-11-21.
//

#include "Skybox.h"
#include "Core.h"
#include <stb_image_aug.h>

#include <GL/glew.h>

namespace internal {
    // loads a cubemap texture from 6 individual texture faces
    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front)
    // -Z (back)
    // -------------------------------------------------------
    unsigned int loadCubemap(std::vector<std::string> faces) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++) {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            } else {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
}// namespace internal

namespace voxie {
    Skybox::Skybox(const Handle &handle, std::shared_ptr<Shader> shader)
        : NodeWrapper(handle), cube({0, 0, 0}, {1, 1, 1}) {
        helper::AddComponent(handle, std::move(shader));
        Init();
    }

    void Skybox::Init() {
        cube.DisableMaterials();
        cube.DisableNormals();
        cube.GenerateVertexAttributes();
        cube.CreateRenderBuffers();
        cube.SetVertexAttrib(3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);

        cubemapTexture = internal::loadCubemap({
            BASE_PATH + SPRITES + "/skybox/right.jpg",
            BASE_PATH + SPRITES + "/skybox/left.jpg",
            BASE_PATH + SPRITES + "/skybox/top.jpg",
            BASE_PATH + SPRITES + "/skybox/bottom.jpg",
            BASE_PATH + SPRITES + "/skybox/front.jpg",
            BASE_PATH + SPRITES + "/skybox/back.jpg"}
        );

        auto shader = GetShader();
        shader->use();
        shader->setInt("skybox", 0);
    }

    void Skybox::encode(YAML::Node &node) const {
        // Havent yet implemented this...
    }

    bool Skybox::decode(const YAML::Node &node) {
        // Havent yet implemented this...
        return true;
    }

    auto Skybox::GetPreDrawAction(const std::shared_ptr<Shader> &shader, const std::shared_ptr<Camera> &camera) const {
        return [=]() {
            glDepthFunc(GL_LEQUAL);
            shader->use();
            auto view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
            shader->setMat4("view", view);
            shader->setMat4("projection", camera->GetProjectionMatrix());
        };
    }

    auto Skybox::GetPostDrawAction() const {
        return []() {
            glDepthFunc(GL_LESS);
        };
    }

    RenderingConfig Skybox::GetRenderingConfig() const {
        return {
                GetShader(),
                GetVertexBufferArray(),
                GetPreDrawAction(GetShader(), Engine::GetEngine().GetCamera()),
                GetPostDrawAction(),
                [&]() {
                    glBindVertexArray(GetVertexBufferArray()->VAO);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
                    glDrawArrays(GL_TRIANGLES, 0, GetVertexBufferArray()->nrVertex);
                    glBindVertexArray(0);
                }};
    }

    std::shared_ptr<VertexBufferArray> Skybox::GetVertexBufferArray() const {
        return cube.GetVertexBufferArray();
    }

}// namespace voxie
