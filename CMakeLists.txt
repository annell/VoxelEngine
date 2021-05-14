cmake_minimum_required(VERSION 3.17)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 17)

# Add header file
set(GLEW_H /usr/local/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /usr/local/Cellar/glfw/3.3.3/include/GLFW)
set(GLM_H /usr/local/Cellar/glm/0.9.9.8/include/GLM)
set(FT_FREETYPE_H /usr/local/Cellar/freetype/2.10.2/include/freetype2)
set(SOIL_H /opt/local/include/SOIL)
include_directories(${GLEW_H} ${GLFW_H} ${GLM_H} ${FT_FREETYPE_H} ${SOIL_H})

# Add target link
set(GLEW_LINK /usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.3/lib/libglfw.3.dylib)
set(SOIL_LINK /opt/local/lib/libSOIL.dylib)
set(FREETYPE_LINK /usr/local/Cellar/freetype/2.10.2/lib/libfreetype.6.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK} ${FREETYPE_LINK} ${SOIL_LINK})

# Execute compilation command
set(SOURCE_FILES Application.cpp)
add_executable(VoxelEngine
        Core.h
        Application.cpp
        Engine.cpp
        Engine.h
        rendering/Shader.h
        rendering/Camera.h
        rendering/VertexBufferArray.h
        rendering/RenderingHandler.cpp
        rendering/RenderingHandler.h
        external/ogt_vox.h
        utility/ModelLoader.h
        utility/TextHandler.h
        utility/Delegate.h
        input/MouseHandler.cpp
        input/MouseHandler.h
        input/KeyboardHandler.cpp
        input/KeyboardHandler.h
        entities/Cube.h
        entities/Lightsource.cpp
        entities/Chunk.cpp
        entities/Chunk.h
        entities/EntityComponentSystem.cpp
        entities/EntityComponentSystem.h)

include_directories(. entities utility input rendering core)

if (APPLE)
    target_link_libraries(VoxelEngine "-framework OpenGL")
    target_link_libraries(VoxelEngine "-framework GLUT")
endif()
