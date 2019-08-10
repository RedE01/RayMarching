project "GLFW"
	kind "StaticLib"
	language "C"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files { "include/GLFW/**.h", "src/**.c", "src/**.h"}

	includedirs { "include"}

	staticruntime "On"

	defines {
		"_GLFW_X11"
	}