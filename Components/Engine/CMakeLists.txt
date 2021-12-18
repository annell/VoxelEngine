cmake_minimum_required(VERSION 3.17)
project(Engine)

# Add header file
find_package(PkgConfig REQUIRED)
pkg_search_module(GLEW REQUIRED GLEW)
set(GLEW_DYN_LIB ${GLEW_INCLUDE_DIRS}/../lib/libGLEW.2.2.dylib)

set(FT_FREETYPE_H /usr/local/Cellar/freetype/2.10.2/include/freetype2)
set(SOIL_H /opt/local/include/SOIL)

set(SOIL_LINK /opt/local/lib/libSOIL.dylib)
set(FREETYPE_LINK /usr/local/Cellar/freetype/2.10.2/lib/libfreetype.6.dylib)

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
        ../../resources/*.cpp
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


include(ExternalProject)
ExternalProject_Add(yaml-cpp
        GIT_REPOSITORY git@github.com:jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.7.0
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        LOG_DOWNLOAD ON
        LOG_INSTALL ON
        )

ExternalProject_Add(glfw3
        GIT_REPOSITORY git@github.com:glfw/glfw.git
        GIT_TAG 3.3.6
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        LOG_DOWNLOAD ON
        LOG_INSTALL ON
        )

ExternalProject_Add(glm
        GIT_REPOSITORY git@github.com:g-truc/glm.git
        GIT_TAG 0.9.9.8
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/glm ${EXTERNAL_INSTALL_LOCATION}/include/glm
        LOG_DOWNLOAD ON
        LOG_INSTALL ON
        )

add_library(${PROJECT_NAME} STATIC
        ${CPP_FILES}
        )

add_dependencies(${PROJECT_NAME}
        yaml-cpp
        glfw3
        glm
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        ${EXTERNAL_INSTALL_LOCATION}/include
        ${GLEW_INCLUDE_DIRS}
        ${FT_FREETYPE_H}
        ${SOIL_H}
        )

if(APPLE)
    set(OSX_GLFW_DEP
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${EXTERNAL_INSTALL_LOCATION}/lib/libyaml-cpp.a
        ${EXTERNAL_INSTALL_LOCATION}/lib/libglfw3.a
        ${GLEW_DYN_LIB}
        ${FREETYPE_LINK}
        ${SOIL_LINK}
        ${OSX_GLFW_DEP}
        "-framework GLUT"
        )

include_directories(${Engine_INCLUDE_DIRS})
