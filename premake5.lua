
include "dependencies.lua"

workspace "Snowcrash Suite"
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



    targetdir ("build/%{prj.name}/%{cfg.longname}")
    objdir ("build/Obj/%{prj.name}/%{cfg.longname}")

-- this is wacky build system stuff to make the premake for GLFW work	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "External"
    include "external/glfw"
    include "external/imgui"
group ""

group "Tools"
    include "snowcrash"
    include "runtime"
    include "editor"
    include "testing"
    -- include "editor"
group ""


