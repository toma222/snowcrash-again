
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

    targetdir ("build/%{prj.name}/%{cfg.longname}")
    objdir ("build/Obj/%{prj.name}/%{cfg.longname}")

group "Tools"
    include "snowcrash"
    include "runtime"
    -- include "editor"
group ""

group "External"
    include "external/glfw"
group ""
