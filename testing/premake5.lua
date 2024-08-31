
project "Testing"
    kind "ConsoleApp"
    files "src/**"

    includedirs 
    {
        "%{wks.location}/snowcrash/src",
		"%{wks.location}/external/glfw/include",
        "src"
    }

    links
    {
        "snowcrash",
		"glfw"
    }

    
    buildoptions 
    {
        "-static-libstdc++",
        "-ggdb"
    }
