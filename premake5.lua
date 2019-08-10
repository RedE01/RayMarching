solution "RayMarching"
	configurations { "Debug", "Release" }

include "vendor/GLEW"
include "vendor/GLFW"

project "RayMarching"
	kind "ConsoleApp"
	language "C++"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	includedirs {"vendor/GLEW/include", "vendor/GLFW/include"}

	files { "src/**.h", "src/**.cpp"}

	-- defines {
	-- 	"GLEW_STATIC"
	-- }

	links {
		"GLEW",
		"GLFW",
		"X11",
		"dl",
		"GL",
		"pthread"
	}

	filter "configurations:Release"
		optimize "On"