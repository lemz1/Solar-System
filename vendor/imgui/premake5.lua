project "ImGui"
	kind "StaticLib"
	language "C++"
    staticruntime "off"

	includedirs 
	{
		"imgui",
        "%{wks.location}/vendor/glfw/glfw/include"
	}

	files
	{
		"imgui/imconfig.h",
		"imgui/imgui.h",
		"imgui/imgui.cpp",
		"imgui/imgui_draw.cpp",
		"imgui/imgui_internal.h",
		"imgui/imgui_tables.cpp",
		"imgui/imgui_widgets.cpp",
		"imgui/imstb_rectpack.h",
		"imgui/imstb_textedit.h",
		"imgui/imstb_truetype.h",
		"imgui/imgui_demo.cpp",
		"imgui/backends/imgui_impl_glfw.h",
		"imgui/backends/imgui_impl_glfw.cpp",
		"imgui/backends/imgui_impl_wgpu.h",
		"imgui/backends/imgui_impl_wgpu.cpp"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter {}