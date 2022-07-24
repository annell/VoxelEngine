#pragma once
#include <memory>

namespace voxie {

    class RenderingInterface {
    public:
        static void Init();
        static void Shutdown();
        static std::shared_ptr<class Window> CreateWindow();
        static void Draw(std::shared_ptr<class VertexBufferArray>);
        static void NewFrame();
        static void RenderFrame(class Window *);
        static float GetTime();
        static bool ShouldClose(class Window *);
    };
}// namespace voxie