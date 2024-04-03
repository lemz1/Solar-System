cmake_minimum_required(VERSION 3.10)
project(GLFW3WebGPU)

set(SOURCES
    "glfw3webgpu/glfw3webgpu.c"
)

set(HEADERS
    "glfw3webgpu/glfw3webgpu.h"
)

add_library(GLFW3WebGPU STATIC ${SOURCES} ${HEADERS})

target_include_directories(GLFW3WebGPU PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/vendor/webgpu/include
    ${CMAKE_SOURCE_DIR}/vendor/glfw/glfw/include
)
