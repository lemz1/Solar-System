project "Solar-System"
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
            "mkdir \"%{cfg.targetdir}/Assets\"",
            "{COPY} \"%{prj.location}Assets\" \"%{cfg.targetdir}/Assets\"",
            "{COPY} \"%{wks.location}/vendor/webgpu/bin/windows-x86_64/wgpu_native.dll\" \"%{cfg.targetdir}\"",
        }

        

    filter "system:linux"
        local current_directory = os.getcwd()
        local absWksPath = current_directory:match("(.+)%/[^%/]+$")

        postbuildcommands
        {
            "{COPY} \""..absWksPath.."/Solar-System/Assets\" \"%{cfg.targetdir}\"",
            "{COPY} \""..absWksPath.."/vendor/webgpu/bin/linux-x86_64/libwgpu_native.so\" \"%{cfg.targetdir}\"",
        }

    filter {}
