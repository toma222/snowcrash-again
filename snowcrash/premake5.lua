
project "snowcrash"
    kind "StaticLib"

    files
    {
        "src/**.cpp",
        "src/**.hpp"
    }

    language "C++"
    cppdialect "C++17"

    includedirs
    {
        "src",
        "%{wks.location}/external",
        "%{wks.location}/external/tinyobjloader",
		"%{wks.location}/external/glfw/include",
        "%{wks.location}/external/glm",
        "%{wks.location}/external/VulkanMemoryAllocator/include",
        "%{IncludeDir.VulkanSDK}",
        "%{wks.location}/external/imgui",
        "%{wks.location}/external/imgui/backends"
    }

	links
	{
		"glfw",
        "%{Library.Vulkan}",
        "mingw32",
        "opengl32",
        "gdi32",
        "imgui"
    }

    linkoptions
    {
        "-static-libstdc++",
        "-static-libgcc"
    }

    filter "configurations:Debug"
        defines "SC_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "SC_RELEASE"
        runtime "Release"
        optimize "on"

