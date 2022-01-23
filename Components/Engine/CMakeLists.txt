cmake_minimum_required(VERSION 3.17)
project(Engine)

# Add header file
find_package(PkgConfig REQUIRED)
pkg_search_module(GLEW REQUIRED GLEW)
set(GLEW_DYN_LIB ${GLEW_INCLUDE_DIRS}/../lib/libGLEW.a)

find_package(Freetype REQUIRED)

set(SOIL_H /opt/local/include/SOIL)

set(SOIL_LINK /opt/local/lib/libSOIL.a)

# Execute compilation command
file(GLOB CPP_FILES
        *.cpp
        entities/*.cpp
        components/*.cpp
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

file(GLOB H_FILES
        *.h
        entities/*.h
        components/*.h
        external/imgui/*.h
        external/ImGuizmo/*.h
        external/rapidjson/*.h
        external/stb/*.h
        external/vox/*.h
        input/*.h
        rendering/*.h
        utility/*.h
        )

set(${PROJECT_NAME}_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/.
        ${PROJECT_SOURCE_DIR}/entities
        ${PROJECT_SOURCE_DIR}/components
        ${PROJECT_SOURCE_DIR}/utility
        ${PROJECT_SOURCE_DIR}/input
        ${PROJECT_SOURCE_DIR}/rendering
        ${PROJECT_SOURCE_DIR}/external/imgui
        ${PROJECT_SOURCE_DIR}/external/ImGuizmo
        ${PROJECT_SOURCE_DIR}/external/stb
        ${PROJECT_SOURCE_DIR}/external/vox
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

target_include_directories(${PROJECT_NAME}
        PUBLIC ${Engine_INCLUDE_DIRS}
        PUBLIC ${GLEW_INCLUDE_DIRS}
        PRIVATE ${EXTERNAL_INSTALL_LOCATION}/include
        PRIVATE ${FREETYPE_INCLUDE_DIRS}
        PRIVATE ${SOIL_H}
        )

if(APPLE)
    set(OSX_GLFW_DEP
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework GLUT")
endif()

target_link_libraries(${PROJECT_NAME}
        PRIVATE ${EXTERNAL_INSTALL_LOCATION}/lib/libyaml-cpp.a
        PRIVATE ${EXTERNAL_INSTALL_LOCATION}/lib/libglfw3.a
        PRIVATE ${GLEW_DYN_LIB}
        PRIVATE ${FREETYPE_LIBRARIES}
        PRIVATE ${SOIL_LINK}
        PRIVATE ${OSX_GLFW_DEP}
        )

include_directories(${Engine_INCLUDE_DIRS})

if (DO_INSTALL)
    install(TARGETS ${PROJECT_NAME} DESTINATION lib)
    install(DIRECTORY ${Engine_INCLUDE_DIRS}
            DESTINATION include/Voxie
            FILES_MATCHING
            PATTERN "*.h"
            )
endif()