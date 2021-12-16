include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Engine"
	architecture "x64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"]		= "%{wks.location}/Kross/vendor/GLFW/include"
IncludeDir["Glad"]		= "%{wks.location}/Kross/vendor/Glad/include"
IncludeDir["ImGui"]		= "%{wks.location}/Kross/vendor/imgui"
IncludeDir["glm"]		= "%{wks.location}/Kross/vendor/glm"
IncludeDir["stb_image"]	= "%{wks.location}/Kross/vendor/stb_image"
IncludeDir["spdlog"]	= "%{wks.location}/Kross/vendor/spdlog/include"
IncludeDir["entt"]		= "%{wks.location}/Kross/vendor/entt"
IncludeDir["yaml"]		= "%{wks.location}/Kross/vendor/serializer/yaml/include"
IncludeDir["Kross"]		= "%{wks.location}/Kross"

group "Dependencies"
	include "Kross/vendor/GLFW"
	include "Kross/vendor/Glad"
	include "Kross/vendor/imgui"
	include "Kross/vendor/serializer/yaml"
	include "Kross"
group ""

include "Editor"
