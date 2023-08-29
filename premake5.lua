-- premake5.lua
workspace "Yak"
    architecture "x64"
    configurations
    { 
        "Debug",
        "Release"
    }
    startproject "Yak"

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/glfw"
	include "vendor/imgui"
group ""

project "Yak"
    kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir)
	objdir ("bin/" .. outputdir .. "/temp")

    pchheader "yakpch.h"
	pchsource "yak/yakpch.cpp"

	files
	{
		"yak/**.h",
		"yak/**.cpp",
        "vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl"
	}

    includedirs
    {
        "yak",
        "vendor/stb_image",
        "vendor/glfw/include",
        "vendor/imgui",
        "vendor/glm"
    }

    links {
        "GLFW",
        "ImGui"
    }

    filter { "system:macosx" }
        defines { "GL_SILENCE_DEPRECATION" }
        linkoptions { "-framework OpenGL -framework Cocoa -framework IOKit" }
   
	filter "configurations:Debug"
		defines "GLCORE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GLCORE_RELEASE"
		runtime "Release"
        optimize "on"
