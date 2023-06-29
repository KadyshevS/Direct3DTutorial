workspace "KDEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}"

IncludeDir = {}
IncludeDir["ImGui"] = "KDEngine/vendor/imgui"

group "Dependencies"
	include "KDEngine/vendor/imgui"
group ""

project "KDEngine"
	location "KDEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	characterset "MBCS"

	targetdir ("%{prj.name}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{prj.name}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.inl",
		"%{prj.name}/src/**.hlsl",

		"%{prj.name}/include/**.h",
		"%{prj.name}/include/**.hpp",
		"%{prj.name}/include/**.inl",
		"%{prj.name}/include/**.hlsl",

		"%{prj.name}/lib/**.lib",
	}

	includedirs
	{
		"%{prj.name}",
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{prj.name}/include",
	}

	links 
	{
		"ImGui"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"bin/" .. outputdir .. "/Sandbox/\""),
			("{COPY} %{cfg.buildtarget.relpath} \"bin/" .. outputdir .. "/KDEditor/\""),
			("{COPYDIR} assets \"bin/" .. outputdir .. "/Sandbox/assets\""),
			("{COPYDIR} assets \"bin/" .. outputdir .. "/KDEditor/assets\"")
		}

	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "on"
		
project "Sandbox"
	location "Sandbox"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	characterset "MBCS"

	targetdir ("KDEngine/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("KDEngine/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"KDEngine/include",
		"KDEngine/vendor",
		"%{prj.name}",
		"%{prj.name}/src"
	}
	
	links
	{
		"KDEngine"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "on"

project "KDEditor"
	location "KDEditor"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	characterset "MBCS"

	targetdir ("KDEngine/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("KDEngine/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"KDEngine/include",
		"KDEngine/vendor",
		"%{prj.name}",
		"%{prj.name}/src"
	}
	
	links
	{
		"KDEngine"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "on"