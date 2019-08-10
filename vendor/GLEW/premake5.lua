project "GLEW"
	kind "StaticLib"
	language "C"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files { "include/GL/**.h", "src/**.c"}

	includedirs { "include"}

	staticruntime "On"

	defines { "GLEW_STATIC"}