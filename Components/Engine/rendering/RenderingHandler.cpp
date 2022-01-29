//
// Created by Stefan Annell on 2021-05-13.
//

#include "RenderingHandler.h"

#include "Core.h"
#include "Shader.h"
#include "VertexBufferArray.h"
#include <utility>

namespace voxie {

    void RenderingHandler::Begin() {
    }

    void RenderingHandler::End() {
        for (auto &config : renderingQueue) {
            config.PreDraw();
            config.shader->use();
            config.Draw();
            config.PostDraw();
        }
        renderingQueue.clear();
    }

    void RenderingHandler::Submit(const RenderingConfig &config) {
        if (config.IsEnabled) {
            renderingQueue.push_back(config);
        }
    }
}// namespace voxie
