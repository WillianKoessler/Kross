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
		"../%{IncludeDir.Kross}/src",
		"../%{IncludeDir.Kross}/vendor",
		"../%{IncludeDir.spdlog}",
		"../%{IncludeDir.entt}",
		"../%{IncludeDir.glm}",
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