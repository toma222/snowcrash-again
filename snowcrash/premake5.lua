
project "snowcrash"
    kind "StaticLib"

    files "src/**"

    includedirs
    {
        "src",
		"%{wks.location}/external/glfw/include"
    }

	links
	{
		"glfw"
	}

