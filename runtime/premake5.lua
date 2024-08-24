
project "Runtime"
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
		"glfw",
        "mingw32"
    }