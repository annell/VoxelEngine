//
// Created by Stefan Annell on 2021-05-13.
//

#pragma once

#include <deque>
#include <functional>
#include <memory>

namespace voxie {

    class Camera;
    class VertexBufferArray;
    class Shader;

    using PreDrawAction = std::function<void()>;
    using PostDrawAction = std::function<void()>;
    using DrawAction = std::function<void()>;

    struct RenderingConfig {
        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexBufferArray> vertexBufferArray;
        PreDrawAction preDraw = []() {};
        PostDrawAction postDraw = []() {};
        DrawAction draw = []() {};
    };

    class RenderingHandler {
    public:
        void Begin();
        void End();

        void Submit(const RenderingConfig &);

    private:
        using RenderingQueue = std::deque<RenderingConfig>;
        RenderingQueue renderingQueue;
    };

}// namespace voxie
