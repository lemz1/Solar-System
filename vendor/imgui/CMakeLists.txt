cmake_minimum_required(VERSION 3.10)
project(ImGUI)

set(SOURCES
    "imgui/imgui.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/backends/imgui_impl_glfw.cpp"
    "imgui/backends/imgui_impl_wgpu.cpp"
)

set(HEADERS
    "imgui/imconfig.h"
    "imgui/imgui.h"
    "imgui/imgui_internal.h"
    "imgui/imstb_rectpack.h"
    "imgui/imstb_textedit.h"
    "imgui/imstb_truetype.h"
    "imgui/backends/imgui_impl_glfw.h"
    "imgui/backends/imgui_impl_wgpu.h"
)

add_library(ImGUI STATIC ${SOURCES} ${HEADERS})

target_include_directories(ImGUI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/vendor/glfw/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/webgpu/include
)

target_include_directories(ImGUI INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR} # if someone wants to do #include <imgui/{file}>
)
