	project "Kross"
		kind "StaticLib"
		--kind "SharedLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
		objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

		pchheader "Kross_pch.h"
		pchsource "src/Kross_pch.cpp"

		files
		{
			"src/**.h",
			"src/**.cpp",
			"vendor/stb_image/**.h",
			"vendor/stb_image/**.cpp",
			"vendor/glm/glm/**.hpp",
			"vendor/glm/glm/**.inl",
			"vendor/ImGuizmo/ImGuizmo.h",
			"vendor/ImGuizmo/ImGuizmo.cpp",
		}

		defines
		{
			"KROSS_BUILD",
			"KROSS_GLAD",
			"KROSS_DEBUG",
			"KROSS_FATAL_BREAK",
			"KROSS_ENABLE_ASSERTS",
			"YAML_CPP_STATIC_DEFINE"
			-- "KROSS_DLL",
			-- "KROSS_BUILD_DLL"
		}

		includedirs
		{
			"vendor",
			"src",
			"%{IncludeDir.GLFW}",		
			"%{IncludeDir.Glad}",		
			"%{IncludeDir.ImGui}",		
			"%{IncludeDir.glm}",		
			"%{IncludeDir.stb_image}",	
			"%{IncludeDir.spdlog}",	
			"%{IncludeDir.entt}",		
			"%{IncludeDir.ImGuizmo}",		
			"%{IncludeDir.yaml}",		


			--"vendor/GLFW/include",
			--"vendor/Glad/include",
			--"vendor/imgui",
			--"vendor/glm",
			--"vendor/stb_image",
			--"vendor/spdlog/include",
			--"vendor/entt",
			--"vendor/serializer/yaml"
		}

		links 
		{ 
			"GLFW",
			"Glad",
			"ImGui",
			"yaml-cpp",
			"opengl32.lib",
		}

		-- postbuildcommands {
		-- 	"copy /Y \"$(TargetDir)$(ProjectName).dll\" \"$(SolutionDir)bin\\%{outputdir}\\barebones\""
		--   }

		filter "files:vendor/ImGuizmo/**.cpp"
			flags { "NoPCH" }

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
