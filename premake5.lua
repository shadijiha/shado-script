workspace "shado-script"
	architecture "x64"
	startproject "shado-script-tests"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

local outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "shado-script"
	location "shado-script"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp"
	}

	includedirs
	{
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"SHADO_PLATFORM_WINDOWS"
		}
	
		postbuildcommands
		{
			
		}
	
	filter "configurations:Debug"
		defines "SHADO_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "SHADO_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SHADO_DIST"
		optimize "Full"


-- For testing
project "shado-script-tests"
	location "shado-script-tests"
	kind "ConsoleApp"

	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp"
	}

	includedirs
	{
		"shado-script/src"
	}

	links
	{
		"shado-script"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

	filter "configurations:Debug"
		defines "SHADO_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "SHADO_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SHADO_DIST"
		optimize "Full"