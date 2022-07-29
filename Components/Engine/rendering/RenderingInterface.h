#pragma once
#include <memory>

namespace voxie {

    enum RenderingType {
        Texture0,
        TextureCubeMap,
        TextureType2D,
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
        Less,
        LessEqual,
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

        static void DepthFunction(RenderingType type);

        static void GenerateTexture(int count, unsigned int *textureID);
        static void BindTexture(RenderingType type, unsigned int textureID);
        static void ActivateTexture(RenderingType);
        static void TextureParameter(RenderingType target, RenderingType paramName, RenderingType param);
        static void TextureImage2D(RenderingType, int, RenderingType, int, int, int, RenderingType, RenderingType, const void *);

        struct VertexBuffers {
            unsigned int VBO = 0;
            unsigned int VAO = 0;
        };
        static VertexBuffers CreateBuffers(const std::vector<float> &attributes);
        static void ResetBuffers(const VertexBuffers &);
        static void SetVertexAttrib(int attributes, unsigned int size, int stride, const void *ptr);
    };
}// namespace voxie