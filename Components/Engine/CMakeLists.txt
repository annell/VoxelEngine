cmake_minimum_required(VERSION 3.17)
project(Engine)

set(CMAKE_CXX_STANDARD 17)

# Address sanitizer
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

# Add header file
set(GLEW_H /usr/local/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /usr/local/Cellar/glfw/3.3.3/include/GLFW)
set(GLM_H /usr/local/Cellar/glm/0.9.9.8/include/GLM)
set(YAMLCPP_H /usr/local/Cellar/yaml-cpp/0.6.3_1/include)
set(FT_FREETYPE_H /usr/local/Cellar/freetype/2.10.2/include/freetype2)
set(SOIL_H /opt/local/include/SOIL)
set(${PROJECT_NAME}_INCLUDE_LIBS
        ${GLEW_H}
        ${GLFW_H}
        ${GLM_H}
        ${YAMLCPP_H}
        ${FT_FREETYPE_H}
        ${SOIL_H}
        CACHE INTERNAL "${PROJECT_NAME}: Include Libs" FORCE)

message(STATUS "INCLUDE_DIRS=${Engine_INCLUDE_LIBS}")
include_directories(
    ${Engine_INCLUDE_LIBS}
)

# Add target link
set(GLEW_LINK /usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.3/lib/libglfw.3.dylib)
set(SOIL_LINK /opt/local/lib/libSOIL.dylib)
set(YAMLCPP_LINK /usr/local/Cellar/yaml-cpp/0.6.3_1/lib/libyaml-cpp.0.6.3.dylib)
set(FREETYPE_LINK /usr/local/Cellar/freetype/2.10.2/lib/libfreetype.6.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK} ${YAMLCPP_LINK} ${FREETYPE_LINK} ${SOIL_LINK})

# Execute compilation command
file(GLOB CPP_FILES
        *.cpp
        entities/*.cpp
        external/imgui/*.cpp
        external/ImGuizmo/*.cpp
        external/rapidjson/*.cpp
        external/stb/*.cpp
        external/vox/*.cpp
        input/*.cpp
        rendering/*.cpp
        resources/*.cpp
        utility/*.cpp
        )

set(${PROJECT_NAME}_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/.
        ${PROJECT_SOURCE_DIR}/entities
        ${PROJECT_SOURCE_DIR}/utility
        ${PROJECT_SOURCE_DIR}/input
        ${PROJECT_SOURCE_DIR}/rendering
        ${PROJECT_SOURCE_DIR}/core
        ${PROJECT_SOURCE_DIR}/external/imgui
        ${PROJECT_SOURCE_DIR}/external/ImGuizmo
        ${PROJECT_SOURCE_DIR}/external/stb
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)


add_library(${PROJECT_NAME} SHARED ${CPP_FILES})

include_directories(${Engine_INCLUDE_DIRS})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework GLUT")
endif ()
