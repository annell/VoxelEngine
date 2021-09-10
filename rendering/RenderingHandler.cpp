//
// Created by Stefan Annell on 2021-05-13.
//

#include "RenderingHandler.h"

#include <utility>
#include "Core.h"
#include "Shader.h"
#include "VertexBufferArray.h"

namespace voxie {

    void RenderingHandler::Begin() {
    }

    void RenderingHandler::End() {
        for (auto &config : renderingQueue) {
            config.preDraw();
            config.shader->use();
            config.draw();
            config.postDraw();
        }
        renderingQueue.clear();
    }

    void RenderingHandler::Submit(const RenderingConfig &config) {
        renderingQueue.push_back(config);
    }
}// namespace voxie
