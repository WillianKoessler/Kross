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
IncludeDir["stb_image"]	= "Kross/vendor/stb_image"
IncludeDir["spdlog"]	= "Kross/vendor/spdlog/include"
IncludeDir["entt"]		= "Kross/vendor/entt"
IncludeDir["Kross"]		= "Kross"

group "Dep"
	include "Kross/vendor/GLFW"
	include "Kross/vendor/Glad"
	include "Kross/vendor/imgui"
	include "Kross"
group ""

include "Editor"