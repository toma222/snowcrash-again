
include "dependencies.lua"

workspace "Snowcrash Suite"
    language "C++"
    architecture "x86_64"
    startproject "snowcrash"
    configurations {"Debug", "Release"}

    filter { "configurations:Debug" }
		-- We want debug symbols in our debug config
		symbols "On"

	-- We now only set settings for Release
	filter { "configurations:Release" }
		-- Release should be optimized
		optimize "On"

    filter {}

    links {
        "mingw32",
        "opengl32",
        "gdi32"
    }

    targetdir ("build/%{prj.name}/%{cfg.longname}")
    objdir ("build/Obj/%{prj.name}/%{cfg.longname}")

-- this is wacky build system stuff to make the premake for GLFW work	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "External"
    include "external/glfw"
group ""

group "Tools"
    include "snowcrash"
    include "runtime"
    include "testing"
    -- include "editor"
group ""


