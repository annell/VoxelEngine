//
// Created by Stefan Annell on 2021-05-13.
//

#include "RenderingHandler.h"
#include "Core.h"
#include "Shader.h"
#include "VertexBufferArray.h"

namespace voxie {

void RenderingHandler::Begin(std::shared_ptr<Camera> camera) {
    selectedCamera = camera;
}

void RenderingHandler::End() {
    for (auto& config : renderingQueue) {
        config.preDraw();
        config.shader->use();
        glBindVertexArray(config.vertexBufferArray->VAO);
        glDrawArrays(GL_TRIANGLES, 0, config.vertexBufferArray->nrVertex);
        config.postDraw();
    }
    renderingQueue.clear();
}

void RenderingHandler::Submit(const RenderingConfig& config) {
    renderingQueue.push_back(config);
}
}
