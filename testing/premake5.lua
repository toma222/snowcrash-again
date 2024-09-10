
project "Testing"
    kind "ConsoleApp"
    files "src/**"

    language "C++"
    cppdialect "C++17"

    includedirs 
    {
        "%{wks.location}/snowcrash/src",
        "%{wks.location}/external/tinyobjloader",
		"%{wks.location}/external/glfw/include",
        "%{wks.location}/external/entt",
        "src"
    }

    links
    {
        --"snowcrash",
		--"glfw"
    }
    
    linkoptions
    {
        "-static-libstdc++",
        "-static-libgcc",
        "-ggdb"
    }
