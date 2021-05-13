cmake_minimum_required(VERSION 3.17)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 14)

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
set(SOURCE_FILES main.cpp)
add_executable(VoxelEngine 
    main.cpp 
    Shader.h 
    Camera.h 
    Cube.h 
    Lightsource.cpp 
    Chunk.cpp 
    Chunk.h 
    ModelLoader.h 
    external/ogt_vox.h
    TextHandler.h
        Engine.cpp Engine.h MouseHandler.cpp MouseHandler.h KeyboardHandler.cpp KeyboardHandler.h Delegate.h EntityComponentSystem.cpp EntityComponentSystem.h)

if (APPLE)
    target_link_libraries(VoxelEngine "-framework OpenGL")
    target_link_libraries(VoxelEngine "-framework GLUT")
endif()
