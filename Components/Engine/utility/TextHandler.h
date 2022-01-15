#include <map>
#include <string>

#include "Shader.h"
#include <utility>

#include "VertexBufferArray.h"

namespace voxie {
    class TextHandler {
    public:
        TextHandler(int screenWidth, int screenHeight, std::string font, std::shared_ptr<Shader>);
        void Init();
        void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

    private:
        void RenderTextImplementation(std::string text, float x, float y, float scale, glm::vec3 color);
        struct Character {
            unsigned int TextureID;// ID handle of the glyph texture
            glm::ivec2 Size;       // Size of glyph
            glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
            unsigned int Advance;  // Horizontal offset to advance to next glyph
        };

        std::map<char, Character> Characters;
        unsigned int VAO{};
        unsigned int VBO{};
        int ScreenWidth;
        int ScreenHeight;
        std::string Font;

        std::shared_ptr<Shader> FontShader;
        std::shared_ptr<VertexBufferArray> vertexBufferArray{};
    };
}// namespace voxie
