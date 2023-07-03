workspace "KDEngine"
	architecture "x64"
	startproject "KDEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}"

IncludeDir = {}
IncludeDir["ImGui"] = "KDEngine/vendor/imgui"
IncludeDir["YAML"] = "KDEngine/vendor/yaml-cpp/include"

group "Dependencies"
	include "KDEngine/vendor/imgui"
	include "KDEngine/vendor/yaml-cpp"
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
		"%{IncludeDir.YAML}",
		"%{IncludeDir.ImGui}"
	}

	links 
	{
		"ImGui",
		"yaml-cpp"
	}

	libdirs
	{
		"%{prj.name}/lib/Shared"
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
			("{COPY} %{cfg.buildtarget.relpath} \"bin/" .. outputdir .. "/KDEditor/\""),
		}

	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"
		libdirs
		{
			"%{prj.name}/lib/Debug"
		}

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
		libdirs
		{
			"%{prj.name}/lib/Release"
		}

	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "on"
		libdirs
		{
			"%{prj.name}/lib/Release"
		}
		
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
		"%{prj.name}/src",
		"%{IncludeDir.YAML}",
		"%{IncludeDir.ImGui}"
	}
	
	links
	{
		"KDEngine"
	}

	libdirs
	{
		"KDEngine/lib/Shared"
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
		libdirs
		{
			"KDEngine/lib/Debug"
		}

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
		libdirs
		{
			"KDEngine/lib/Release"
		}

	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "on"
		libdirs
		{
			"KDEngine/lib/Release"
		}

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
		"%{prj.name}/src",
		"%{IncludeDir.YAML}",
		"%{IncludeDir.ImGui}"
	}
	
	links
	{
		"KDEngine"
	}

	libdirs
	{
		"KDEngine/lib/Shared"
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
		libdirs
		{
			"KDEngine/lib/Debug"
		}

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
		libdirs
		{
			"KDEngine/lib/Release"
		}

	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "on"
		libdirs
		{
			"KDEngine/lib/Release"
		}