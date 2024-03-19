project "Solar System"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir("%{wks.location}/export/%{cfg.system}/%{cfg.buildcfg}/bin")

    files { "**.h", "**.c", "**.hpp", "**.cpp" }

    includedirs
    {
        "src",
        "%{wks.location}/vendor/glfw/glfw/include",
        "%{wks.location}/vendor/glm/glm",
        "%{wks.location}/vendor/webgpu/include",
        "%{wks.location}/vendor/glfw3webgpu",
    }

    links
    {
        "GLFW",
        "GLFW3WebGPU",
        "%{wks.location}/vendor/webgpu/bin/windows-x86_64/wgpu_native.dll.lib",
    }

    postbuildcommands
    {
        "{COPY} \"%{prj.location}Assets\" \"%{cfg.targetdir}/Assets\"",
        -- "{COPY} \"%{prj.location}*imgui.ini\" \"%{cfg.targetdir}\"",
    }

    filter "system:windows"
        postbuildcommands
        {
            "{COPY} \"%{wks.location}/vendor/webgpu/bin/windows-x86_64/wgpu_native.dll\" \"%{cfg.targetdir}\"",
        }

    filter "system:linux"
        postbuildcommands
        {
            "{COPY} \"%{wks.location}/vendor/webgpu/bin/linux-x86_64/libwgpu_native.so\" \"%{cfg.targetdir}\"",
        }
    
    filter {}
