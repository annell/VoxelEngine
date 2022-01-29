cmake_minimum_required(VERSION 3.17)
project(Voxie)
# You can tweak some common (for all subprojects) stuff here. For example:

# Address sanitizer
option(USE_ASAN "Run the project with ASAN" ON)
if(USE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()


set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
set(CMAKE_CXX_STANDARD 17)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

set(EXTERNAL_INSTALL_LOCATION
        ${CMAKE_BINARY_DIR}/external
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
        )
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)

add_subdirectory(components/Engine)
add_subdirectory(components/Editor)
