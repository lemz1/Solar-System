project "GLFW3WebGPU"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    files { "**.h", "**.c", "**.hpp", "**.cpp" }

    includedirs
    {
        "%{wks.location}/vendor/glfw/glfw/include",
        "%{wks.location}/vendor/webgpu/include",
    }
