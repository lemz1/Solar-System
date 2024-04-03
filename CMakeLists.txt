cmake_minimum_required(VERSION 3.10)
project(Solar-System)

add_definitions(
    -DGLM_ENABLE_EXPERIMENTAL
    -DGLM_FORCE_DEPTH_ZERO_TO_ONE
    -DGLM_FORCE_LEFT_HANDED
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE)
endif()

if(WIN32)
    add_definitions(-DWINDOWS)
endif()

if(UNIX AND NOT APPLE)
    add_definitions(-DLINUX)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/export/${CMAKE_SYSTEM_NAME}")

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glfw3webgpu)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/webgpu)

set(SOURCES
    "src/Main.cpp"
    "src/wgpu/DepthTexture.cpp"
    "src/wgpu/Mesh.cpp"
    "src/wgpu/Texture2D.cpp"
    "src/wgpu/WGPUContext.cpp"
    "src/util/AssetManager.cpp"
    "src/util/Camera.cpp"
    "src/util/CameraController.cpp"
    "src/util/Math.cpp"
    "src/states/PlanetCreator.cpp"
    "src/core/Application.cpp"
    "src/core/Input.cpp"
    "src/core/Window.cpp"
    "src/celestial-bodies/IcoSphere.cpp"
    "src/celestial-bodies/Planet.cpp"
    "src/celestial-bodies/PlanetPipeline.cpp"
)

set(HEADERS
    "src/wgpu/DepthTexture.h"
    "src/wgpu/Mesh.h"
    "src/wgpu/Texture2D.h"
    "src/wgpu/WGPUContext.h"
    "src/util/AssetManager.h"
    "src/util/Camera.h"
    "src/util/CameraController.h"
    "src/util/Math.h"
    "src/util/FractalNoiseSettings.h"
    "src/states/PlanetCreator.h"
    "src/debug/Timer.h"
    "src/core/Application.h"
    "src/core/KeyCode.h"
    "src/core/MouseCode.h"
    "src/core/State.h"
    "src/core/Input.h"
    "src/core/Window.h"
    "src/celestial-bodies/IcoSphere.h"
    "src/celestial-bodies/Planet.h"
    "src/celestial-bodies/PlanetPipeline.h"
)

add_executable(Solar-System ${SOURCES} ${HEADERS})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(MSVC)
        target_compile_options(Solar-System PRIVATE /Od)
    else()
        target_compile_options(Solar-System PRIVATE -O0)
    endif()
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MSVC)
        target_compile_options(Solar-System PRIVATE /Ox)
    else()
        target_compile_options(Solar-System PRIVATE -O3)
    endif()
endif()

target_link_libraries(Solar-System PRIVATE 
    glfw    
    GLFW3WebGPU
    ImGUI
    webgpu
)

target_include_directories(Solar-System PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor/glm/glm
    ${CMAKE_SOURCE_DIR}/vendor/stb_image
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/assets
)

# Make the Solar-System target depend on the copy_assets target
add_dependencies(Solar-System copy_assets)

set(TARGET_FILE_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_copy_webgpu_binaries(Solar-System)