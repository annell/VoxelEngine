cmake_minimum_required(VERSION 3.17)
project(Editor C CXX)

add_executable(${PROJECT_NAME}
        EditorApplication.cpp
        gui/Panel.cpp
        )

include_directories(
        gui
)

add_dependencies(${PROJECT_NAME} Engine)

target_link_libraries(${PROJECT_NAME} PUBLIC Engine)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${Engine_INCLUDE_DIRS}
)
