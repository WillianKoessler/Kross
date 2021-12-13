project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"assets/scripts/native",
		"../%{IncludeDir.Kross}/src",
		"../%{IncludeDir.Kross}/vendor",
		"../%{IncludeDir.spdlog}",
		"../%{IncludeDir.entt}",
		"../%{IncludeDir.glm}",
		"../%{IncludeDir.stb_image}",
		"../%{IncludeDir.ImGui}",
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
			"KROSS_FATAL_BREAK",
			"KROSS_ENABLE_ASSERTS"
			-- "KROSS_DLL"
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