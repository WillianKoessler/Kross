workspace "Engine"
	architecture "x64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"]		= "Kross/vendor/GLFW/include"
IncludeDir["Glad"]		= "Kross/vendor/Glad/include"
IncludeDir["ImGui"]		= "Kross/vendor/imgui"
IncludeDir["glm"]		= "Kross/vendor/glm"
IncludeDir["stb_image"]		= "Kross/vendor/stb_image"
IncludeDir["spdlog"]		= "Kross/vendor/spdlog/include"
IncludeDir["entt"]		= "Kross/vendor/entt"

group "Dep"
	include "Kross/vendor/GLFW"
	include "Kross/vendor/Glad"
	include "Kross/vendor/imgui"
	project "Kross"
		location "Kross"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		pchheader "Kross_pch.h"
		pchsource "Kross/src/Kross_pch.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/vendor/stb_image/**.h",
			"%{prj.name}/vendor/stb_image/**.cpp",
			"%{prj.name}/vendor/glm/glm/**.hpp",
			"%{prj.name}/vendor/glm/glm/**.inl",
		}

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"KROSS_GLAD",
		}

		includedirs
		{
			"%{prj.name}/vendor",
			"%{prj.name}/src",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.stb_image}",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.entt}",
		}

		links 
		{ 
			"GLFW",
			"Glad",
			"ImGui",
			"opengl32.lib",
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"KROSS_PLATFORM_WINDOWS",
				"GLFW_INCLUDE_NONE",
			}

		filter "configurations:Debug"
			defines "KROSS_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "KROSS_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "KROSS_DIST"
			runtime "Release"
			optimize "on"
group ""

project "Editor"
	location "main"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"Kross/src",
		"Kross/vendor",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
	}

	links
	{
		"Kross"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"KROSS_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "KROSS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "KROSS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "KROSS_DIST"
		runtime "Release"
		optimize "on"