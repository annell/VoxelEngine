//
// Created by Stefan Annell on 2021-05-13.
//

#pragma once

#include <memory>
#include <functional>
#include <deque>

namespace voxie {

class Camera;
class VertexBufferArray;
class Shader;

using PreDrawAction = std::function<void ()>;
using PostDrawAction = std::function<void ()>;
using DrawAction = std::function<void ()>;

struct RenderingConfig {
    std::shared_ptr<Shader> shader;
    std::shared_ptr<VertexBufferArray> vertexBufferArray;
    PreDrawAction preDraw = [] () {};
    PostDrawAction postDraw = [] () {};
    DrawAction draw = [] () {};
};

class RenderingHandler {
public:
    void Begin(std::shared_ptr<Camera> camera);
    void End();

    void Submit(const RenderingConfig&);

private:
    using RenderingQueue = std::deque<RenderingConfig>;
    std::shared_ptr<Camera> selectedCamera;
    RenderingQueue renderingQueue;
};

}
