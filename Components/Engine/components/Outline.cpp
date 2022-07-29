//
// Created by Stefan Annell on 2022-04-03.
//

#include "Outline.h"
#include "Core.h"
#include <GL/glew.h>

namespace voxie {

    void Outline::Render() const {
        shader->use();
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        auto model = glm::scale(position->model, glm::vec3(scale, scale, scale));
        shader->setMat4("model", model);

        RenderingInterface::Draw(vertexBufferArray);

        glFrontFace(GL_CW);
        glDisable(GL_CULL_FACE);
    }
}// namespace voxie
