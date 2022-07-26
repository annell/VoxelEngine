#pragma once
#include <memory>

namespace voxie {

    enum RenderingType {
        Texture0,
        TextureCubeMap,
        Triangles,
        TextureMinFilter,
        TextureMagFilter,
        TextureWrapS,
        TextureWrapT,
        TextureWrapR,
        ClampToEdge,
        TextureCubeMapPositiveX,
        TextureCubeMapNegativeX,
        TextureCubeMapPositiveY,
        TextureCubeMapNegativeY,
        TextureCubeMapPositiveZ,
        TextureCubeMapNegativeZ,
        RGB,
        UnsignedByte,
        Linear
    };

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

        static void GenerateTexture(int count, unsigned int *textureID);
        static void BindTexture(RenderingType type, unsigned int textureID);
        static void TextureParameter(RenderingType target, RenderingType paramName, RenderingType param);
        static void TextureImage2D(RenderingType, int, RenderingType, int, int, int, RenderingType, RenderingType, const void *);
    };
}// namespace voxie