workspace "Solar System"
    architecture "x64"
    startproject "Solar System"
    configurations { "Debug", "Release" }

    targetdir("%{wks.location}/export/%{cfg.system}/%{cfg.buildcfg}/libs/%{prj.name}")
    objdir("%{wks.location}/export/%{cfg.system}/%{cfg.buildcfg}/objs/%{prj.name}")

    defines { "GLM_ENABLE_EXPERIMENTAL" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        optimize "Off"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
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

include "Solar System"
