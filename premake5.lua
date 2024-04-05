workspace "Solar-System"
    architecture "x64"
    startproject "Solar-System"
    configurations { "Debug", "Release" }

    targetdir("%{wks.location}/export/%{cfg.system}/%{cfg.buildcfg}/libs/%{prj.name}")
    objdir("%{wks.location}/export/%{cfg.system}/%{cfg.buildcfg}/objs/%{prj.name}")

    defines 
    { 
        "GLM_ENABLE_EXPERIMENTAL",
        "GLM_FORCE_DEPTH_ZERO_TO_ONE",
        "GLM_FORCE_LEFT_HANDED"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        optimize "Off"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "Full"
        symbols "Off"

    filter "system:windows"
		defines { "WINDOWS" }

	filter "system:linux"
		defines { "LINUX" }

    filter {}

group "Dependencies"
    include "vendor/glfw"
    include "vendor/glfw3webgpu"
    include "vendor/imgui"
group ""

project "Solar-System"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir("%{wks.location}/export/%{cfg.system}/%{cfg.buildcfg}/bin")

    files { "src/**.h", "src/**.c", "src/**.hpp", "src/**.cpp" }

    includedirs
    {
        "src",
        "%{wks.location}/vendor/glfw/glfw/include",
        "%{wks.location}/vendor/glm/glm",
        "%{wks.location}/vendor/webgpu/include",
        "%{wks.location}/vendor/glfw3webgpu",
        "%{wks.location}/vendor/stb_image",
    }

    links
    {
        "GLFW",
        "GLFW3WebGPU",
    }

    filter "system:windows"
        links { "%{wks.location}/vendor/webgpu/bin/windows-x86_64/wgpu_native.dll.lib" }

    filter "system:linux"
        libdirs { "%{wks.location}/vendor/webgpu/bin/linux-x86_64" }
        links { "wgpu_native" }


    postbuildcommands
    {
        -- "{COPY} \"%{prj.location}*imgui.ini\" \"%{cfg.targetdir}\"",
    }

    filter "system:windows"
        postbuildcommands
        {
            "mkdir \"%{cfg.targetdir}/assets\"",
            "{COPY} \"%{prj.location}assets\" \"%{cfg.targetdir}/assets\"",
            "{COPY} \"%{wks.location}/vendor/webgpu/bin/windows-%{cfg.architecture}/wgpu_native.dll\" \"%{cfg.targetdir}\"",
        }

        

    filter "system:linux"
        local current_directory = os.getcwd()
        local absWksPath = current_directory:match("(.+)%/[^%/]+$")

        postbuildcommands
        {
            "{COPY} \""..absWksPath.."/assets\" \"%{cfg.targetdir}\"",
            "{COPY} \""..absWksPath.."/vendor/webgpu/bin/linux-%{cfg.architecture}/libwgpu_native.so\" \"%{cfg.targetdir}\"",
        }

    filter {}


